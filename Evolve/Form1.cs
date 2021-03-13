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

        enum EMessageCodes
        {
            EMessageCode_Info,
            EMessageCode_Aborted,
            EMessageCode_Completed,
            EMessageCode_Saved,
            EMessageCode_Error
        }

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void MessageCallback(int msgCode, string message);

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void StartTraining(TrainingSettings settings, [MarshalAs(UnmanagedType.FunctionPtr)] MessageCallback callbackPointer);
        //public static extern int StartTraining(TrainingSettings settings);

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int AbortTraining();

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int NumberEpochs();

        private delegate void DelegateProcessMessage(int msgCode, string text);

        Thread m_workerThread;
        VisualSimulation m_visualSimulation = null;

        public Form1()
        {
            InitializeComponent();
        }

        private void buttonStartTraining_Click(object sender, EventArgs e)
        {
            if (m_workerThread != null && m_workerThread.IsAlive)
            {
                UpdateInfoLog(0, "Thread still alive");
            }
            TrainingSettings settings = new TrainingSettings();

            settings.m_populationSize = (int) numericUpDownPopulationSize.Value;
            settings.m_directPromotion = (int) numericUpDownDirectPromotions.Value;
            settings.m_mutationRate = (float) numericUpDownMutationRate.Value;
            settings.m_roundsParentSelection = (int) numericUpDownParentSel.Value;
            settings.m_maxEpochs = (int)numericUpDownMaxEpochs.Value;
            settings.m_periodSave = (int)numericUpDownSaveFreq.Value;
            settings.m_periodOutput = (int) numericUpDownOutputFreq.Value;
            settings.m_numberThreads = (int) numericUpDownNumThreads.Value;
            settings.m_resultName = textBoxResultsName.Text;

            m_workerThread = new Thread(() => WorkerThreadEntryPoint(settings, OnProgressCallback));
            m_workerThread.Start();
        }

        private void buttonAbortTraining_Click(object sender, EventArgs e)
        {
            AbortTraining();
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
                case EMessageCodes.EMessageCode_Aborted:
                    color = Color.Red;
                    break;
                case EMessageCodes.EMessageCode_Completed:
                    color = Color.Green;
                    break;
                case EMessageCodes.EMessageCode_Saved:
                    color = Color.Blue;
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

        public static void WorkerThreadEntryPoint(TrainingSettings settings, MessageCallback callback)
        {
            StartTraining(settings, callback);
        }

        private void buttonLoadSettings_Click(object sender, EventArgs e)
        {
        }

        private void buttonVisualize_Click(object sender, EventArgs e)
        {
            if (m_visualSimulation == null)
            {
                m_visualSimulation = new VisualSimulation();
            }
            if (m_visualSimulation.IsDisposed)
            {
                m_visualSimulation = new VisualSimulation();
            }
            m_visualSimulation.Show();
        }
    }
}
