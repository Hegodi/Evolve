using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.IO;
using System.Runtime.InteropServices;

namespace Evolve
{
    public partial class Form1 : Form
    {

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
                EvolveKernelAPI.AbortTraining();
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
                        UpdateInfoLog((int)EvolveKernelAPI.EMessageCodes.EMessageCode_Warning, "Results in '" + filename + "' will be overriden");
                        string[] files = Directory.GetFiles(filename);
                        for (int i = 0; i < files.Length; i++)
                        {
                            File.Delete(files[i]);
                        }
                    }
                    else
                    {
                        UpdateInfoLog((int)EvolveKernelAPI.EMessageCodes.EMessageCode_Error, "Folder named '" + filename + "' already exist");
                        return;
                    }
                }

                if (createDirectory)
                {
                    Directory.CreateDirectory(filename);
                    UpdateInfoLog((int)EvolveKernelAPI.EMessageCodes.EMessageCode_InfoHigh, "Created directory '" + filename + "'");
                }
                else
                {
                }

                EvolveKernelAPI.TrainingSettings settings = new EvolveKernelAPI.TrainingSettings();
                settings.m_populationSize = (int)numericUpDownPopulationSize.Value;
                settings.m_directPromotion = (int)numericUpDownDirectPromotions.Value;
                settings.m_mutationRate = (float)numericUpDownMutationRate.Value;
                settings.m_roundsParentSelection = (int)numericUpDownParentSel.Value;
                settings.m_maxEpochs = (int)numericUpDownMaxEpochs.Value;
                settings.m_periodSave = (int)numericUpDownSaveFreq.Value;
                settings.m_periodOutput = (int)numericUpDownOutputFreq.Value;
                settings.m_numberThreads = (int)numericUpDownNumThreads.Value;
                settings.m_randomSeed = (int) numericUpDownRandomSeed.Value;
                settings.m_resultName = filename;

                settings.m_weightDistanceTraveled = (float) weightDistanceTraveled.Value;
                settings.m_weightCollisionsWithGround = (float) weightCollisionsGround.Value;
                settings.m_weightCollisionsWithObstacles = (float)weightCollisionsObstacles.Value;
                settings.m_weightMaxHeight = (float) weightMaxHeight.Value;
                settings.m_weightAverageHeight = (float) weightAverageHeight.Value;
                settings.m_weightLoadDistanceTraveled = (float) weightLoadDistanceTraveled.Value;
                settings.m_weightLoadCollisionsWithGround = (float) weightLoadCollisionsGround.Value;
                settings.m_weightNumberNodes = (float)weightNumberNodes.Value;
                settings.m_weightReactivity = (float)weightReactivity.Value;

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
            EvolveKernelAPI.EMessageCodes msgCode = (EvolveKernelAPI.EMessageCodes)msgCodeInt;

            textBoxInfo.SelectionStart = textBoxInfo.TextLength;
            textBoxInfo.SelectionLength = 0;

            Color color = Color.Black;
            switch (msgCode)
            {
                case EvolveKernelAPI.EMessageCodes.EMessageCode_Info:
                    color = Color.Black;
                    break;
                case EvolveKernelAPI.EMessageCodes.EMessageCode_InfoHigh:
                    color = Color.Blue;
                    break;
                case EvolveKernelAPI.EMessageCodes.EMessageCode_Warning:
                    color = Color.Orange;
                    break;
                case EvolveKernelAPI.EMessageCodes.EMessageCode_Error:
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

        private static void WorkerThreadEntryPoint(EvolveKernelAPI.TrainingSettings settings, EvolveKernelAPI.MessageCallback callback)
        {
            EvolveKernelAPI.StartTraining(settings, callback);
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
            listBoxResults.Items.Clear();
            listBoxResults.SelectedIndex = -1;
            if (folderBrowserDialogResults.ShowDialog() != DialogResult.OK)
            {
                return;
            }

            m_resultFilenames = Directory.GetFiles(folderBrowserDialogResults.SelectedPath, "*.gar");
            for (int i = 0; i < m_resultFilenames.Length; i++)
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
                EvolveKernelAPI.GraphicsSimulationSettings settings;
                settings.m_dnaFilename = m_resultFilenames[listBoxResults.SelectedIndex];
                m_simulationThread = new Thread(() => SimulationThreadEntryPoint(settings));
                m_simulationThread.Start();
            }
        }
        private static void SimulationThreadEntryPoint(EvolveKernelAPI.GraphicsSimulationSettings settings)
        {
            EvolveKernelAPI.RunGraphicsSimulation(settings);
        }

        private void listBoxResults_SelectedIndexChanged(object sender, EventArgs e)
        {
            richTextBoxResultSelected.Text = "";

            if (listBoxResults.SelectedIndex < 0 || listBoxResults.SelectedIndex >= listBoxResults.Items.Count)
            {
                return;
            }

            IntPtr dataRobot = EvolveKernelAPI.CreateRobotInfo(m_resultFilenames[listBoxResults.SelectedIndex]);
            if (dataRobot == null)
            {
                return;
            }

            EvolveKernelAPI.RobotInfo robotInfo = (EvolveKernelAPI.RobotInfo)Marshal.PtrToStructure(dataRobot, typeof(EvolveKernelAPI.RobotInfo));

            richTextBoxResultSelected.AppendText(listBoxResults.SelectedItem.ToString());
            richTextBoxResultSelected.AppendText(Environment.NewLine);
            richTextBoxResultSelected.AppendText("Number Nodes: " + robotInfo.m_numNodes);
            richTextBoxResultSelected.AppendText(Environment.NewLine);
            richTextBoxResultSelected.AppendText("Number Srpings: " + robotInfo.m_numSprings);
            richTextBoxResultSelected.AppendText(Environment.NewLine);
            richTextBoxResultSelected.AppendText(Environment.NewLine);

            if (robotInfo.m_nodes != IntPtr.Zero) 
            {
                richTextBoxResultSelected.AppendText("NODES:");
                richTextBoxResultSelected.AppendText(Environment.NewLine);
                int structNodeInfoSize = Marshal.SizeOf(typeof(EvolveKernelAPI.NodeInfo));
                for (int i = 0; i < robotInfo.m_numNodes; ++i)
                {
                    IntPtr data = new IntPtr(robotInfo.m_nodes.ToInt64() + structNodeInfoSize * i);
                    if (data == IntPtr.Zero)
                    {
                        Console.WriteLine("ERROR: data == null");
                        continue;
                    }
                    EvolveKernelAPI.NodeInfo nodeInfo = (EvolveKernelAPI.NodeInfo)Marshal.PtrToStructure(data, typeof(EvolveKernelAPI.NodeInfo));
                    richTextBoxResultSelected.AppendText("Node " + i);
                    richTextBoxResultSelected.AppendText(Environment.NewLine);
                    richTextBoxResultSelected.AppendText("     Position: " + nodeInfo.m_posX + ", " + nodeInfo.m_posY);
                    richTextBoxResultSelected.AppendText(Environment.NewLine);
                    richTextBoxResultSelected.AppendText("     Radius: " + nodeInfo.m_radius);
                    richTextBoxResultSelected.AppendText(Environment.NewLine);
                    richTextBoxResultSelected.AppendText("     Elastic Coefficient: " + nodeInfo.m_elasticCoefficient);
                    richTextBoxResultSelected.AppendText(Environment.NewLine);
                    richTextBoxResultSelected.AppendText("     Friction Coefficient: " + nodeInfo.m_frictionCoefficient);
                    richTextBoxResultSelected.AppendText(Environment.NewLine);
                }

                richTextBoxResultSelected.AppendText(Environment.NewLine);
                richTextBoxResultSelected.AppendText("SPRINGS:");
                richTextBoxResultSelected.AppendText(Environment.NewLine);
                for (int i = 0; i < robotInfo.m_numSprings; ++i)
                {
                    int structSpringInfoSize = Marshal.SizeOf(typeof(EvolveKernelAPI.SpringInfo));
                    IntPtr data = new IntPtr(robotInfo.m_springs.ToInt64() + structSpringInfoSize * i);
                    if (data == IntPtr.Zero)
                    {
                        Console.WriteLine("ERROR: data == null");
                        continue;
                    }
                    EvolveKernelAPI.SpringInfo springInfo = (EvolveKernelAPI.SpringInfo)Marshal.PtrToStructure(data, typeof(EvolveKernelAPI.SpringInfo));
                    richTextBoxResultSelected.AppendText("Spring " + i + ":  Node " + springInfo.m_indNodeStart + " - Node " + springInfo.m_indNodeEnd);
                    richTextBoxResultSelected.AppendText(Environment.NewLine);
                    richTextBoxResultSelected.AppendText("     Length: " + springInfo.m_length);
                    richTextBoxResultSelected.AppendText(Environment.NewLine);
                    richTextBoxResultSelected.AppendText("     Spring Constant: " + springInfo.m_springConstant);
                    richTextBoxResultSelected.AppendText(Environment.NewLine);
                    if (springInfo.m_deltaLength > 0.0)
                    {
                        richTextBoxResultSelected.AppendText("     Length Variation Factor: " + springInfo.m_deltaLength);
                        richTextBoxResultSelected.AppendText(Environment.NewLine);
                        richTextBoxResultSelected.AppendText("     Period Variation: " + springInfo.m_period);
                        richTextBoxResultSelected.AppendText(Environment.NewLine);

                    }
                }
            }

            EvolveKernelAPI.DisposeRobotInfo(dataRobot);
        }
    }
}
