using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;
using System.IO;

namespace Evolve
{
    public partial class Form1 : Form
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct TrainingSettings
        {
            public int m_populationSize;
	        public int m_directPromotion;
	        public float m_mutationRate;
	        public int m_roundsParentSelection;
	        public int m_maxEpochs;
	        public int m_periodSave;
	        public int m_periodOutput;
	        public int m_numberThreads;
            public int m_randomSeed;
            public string m_resultName;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GraphicsSimulationSettings
        {
            public string m_dnaFilename;
        }

        enum EMessageCodes
        {
            EMessageCode_Info,
            EMessageCode_InfoHigh,
            EMessageCode_Warning,
            EMessageCode_Error
        }

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void MessageCallback(int msgCode, string message);

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int StartTraining(TrainingSettings settings, [MarshalAs(UnmanagedType.FunctionPtr)] MessageCallback callbackPointer);

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int AbortTraining();

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int NumberEpochs();

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RunGraphicsSimulation(GraphicsSimulationSettings settings);

        private delegate void DelegateProcessMessage(int msgCode, string text);

        Thread m_workerThread;
        Thread m_simulationThread;

        // Results:
        string[] m_resultFilenames = null;

        public Form1()
        {
            InitializeComponent();
            ResetSettings();
            folderBrowserDialogResults.SelectedPath = Application.StartupPath;
            openFileDialogSettings.InitialDirectory = Application.StartupPath;
        }

        private void buttonStartTraining_Click(object sender, EventArgs e)
        {
            if (m_workerThread != null && m_workerThread.IsAlive)
            {
                AbortTraining();
                buttonStartStopTraining.Text = "Start Training";
            }
            else
            {

                string filename = textBoxResultsName.Text;

                bool createDirectory = true;
                if (Directory.Exists(filename))
                {
                    DialogResult result = MessageBox.Show("A folder named '" + filename + "' already exist. Do you want to overwrite it?", "ERROR", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);
                    if (result == DialogResult.Yes)
                    {
                        createDirectory = false;
                        UpdateInfoLog((int)EMessageCodes.EMessageCode_Warning, "Results in '" + filename + "' will be overriden");
                    }
                    else
                    {
                        UpdateInfoLog((int)EMessageCodes.EMessageCode_Error, "Folder named '" + filename + "' already exist");
                        return;
                    }
                }

                if (createDirectory)
                {
                    Directory.CreateDirectory(filename);
                    UpdateInfoLog((int)EMessageCodes.EMessageCode_InfoHigh, "Created directory '" + filename + "'");
                }

                TrainingSettings settings = new TrainingSettings();
                settings.m_populationSize = (int)numericUpDownPopulationSize.Value;
                settings.m_directPromotion = (int)numericUpDownDirectPromotions.Value;
                settings.m_mutationRate = (float)numericUpDownMutationRate.Value;
                settings.m_roundsParentSelection = (int)numericUpDownParentSel.Value;
                settings.m_maxEpochs = (int)numericUpDownMaxEpochs.Value;
                settings.m_periodSave = (int)numericUpDownSaveFreq.Value;
                settings.m_periodOutput = (int)numericUpDownOutputFreq.Value;
                settings.m_numberThreads = (int)numericUpDownNumThreads.Value;
                settings.m_resultName = filename;

                buttonStartStopTraining.Text = "Abort Training";
                m_workerThread = new Thread(() => WorkerThreadEntryPoint(settings, OnProgressCallback));
                m_workerThread.Start();
            }
        }

        private void OnProgressCallback(int msgCode, string message)
        {
            Console.WriteLine(message);
            this.Invoke(new DelegateProcessMessage(UpdateInfoLog), msgCode, message);
        }

        private void UpdateInfoLog(int msgCodeInt, string message)
        {
            EMessageCodes msgCode = (EMessageCodes)msgCodeInt;

            textBoxInfo.SelectionStart = textBoxInfo.TextLength;
            textBoxInfo.SelectionLength = 0;

            Color color = Color.Black;
            switch (msgCode)
            {
                case EMessageCodes.EMessageCode_Info:
                    color = Color.Black;
                    break;
                case EMessageCodes.EMessageCode_InfoHigh:
                    color = Color.Blue;
                    break;
                case EMessageCodes.EMessageCode_Warning:
                    color = Color.Orange;
                    break;
                case EMessageCodes.EMessageCode_Error:
                    color = Color.Red;
                    break;
            }
            textBoxInfo.SelectionColor = color;

            textBoxInfo.AppendText(message);
            textBoxInfo.AppendText(Environment.NewLine);
            textBoxInfo.SelectionColor = textBoxInfo.ForeColor;
            textBoxInfo.ScrollToCaret();
        }

        void ResetSettings()
        {
            numericUpDownPopulationSize.Value = 1000;
            numericUpDownDirectPromotions.Value = 5;
            numericUpDownMutationRate.Value = (decimal)0.001;
            numericUpDownParentSel.Value = 2;
            numericUpDownMaxEpochs.Value = 100;
            numericUpDownSaveFreq.Value = 10;
            numericUpDownOutputFreq.Value = 1;
            numericUpDownNumThreads.Value = 1;
            textBoxResultsName.Text = "NewResults";
        }

        public static void WorkerThreadEntryPoint(TrainingSettings settings, MessageCallback callback)
        {
            StartTraining(settings, callback);
        }

        private void buttonLoadSettings_Click(object sender, EventArgs e)
        {
        }

        private void buttonNewSettings_Click(object sender, EventArgs e)
        {
            ResetSettings();
        }
        private void buttonSaveSettings_Click(object sender, EventArgs e)
        {
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// 
        ///     Results Visualization
        /// 
        private void buttonOpenResults_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialogResults.ShowDialog() != DialogResult.OK)
            {
                return;
            }

            m_resultFilenames = Directory.GetFiles(folderBrowserDialogResults.SelectedPath, "*.gar");
            for (int i=0; i<m_resultFilenames.Length; i++)
            {
                string name = Path.GetFileName(m_resultFilenames[i]);
                listBoxResults.Items.Add(name);
            }

            textBoxResults.Text = folderBrowserDialogResults.SelectedPath;
        }

        private void buttonSimulate_Click(object sender, EventArgs e)
        {
            if (m_simulationThread != null && m_simulationThread.IsAlive)
            {
                MessageBox.Show("Another simulation is already running", "Cannot Run", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            if (listBoxResults.SelectedIndex >= 0 && listBoxResults.SelectedIndex < listBoxResults.Items.Count)
            {
                GraphicsSimulationSettings settings;
                settings.m_dnaFilename = m_resultFilenames[listBoxResults.SelectedIndex];
                m_simulationThread = new Thread(() => SimulationThreadEntryPoint(settings));
                m_simulationThread.Start();
            }
        }
        public static void SimulationThreadEntryPoint(GraphicsSimulationSettings settings)
        {
            RunGraphicsSimulation(settings);
        }
    }
}
