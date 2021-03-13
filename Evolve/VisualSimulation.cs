using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;
using System.Drawing.Drawing2D;
using System.Drawing.Text;

namespace Evolve
{

    public partial class VisualSimulation : Form
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct Node
        {
            public float m_posX;
            public float m_posY;
            public float m_radius;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct Spring
        {
            public float m_p1X;
            public float m_p1Y;
            public float m_p2X;
            public float m_p2Y;
            public float m_length;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct SimulationSettings
        {
            public string m_dnaFilename;
        }


        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetNodesInWorld();

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetNumberSpringsInWorld();
        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetNumberNodesInWorld();

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetSpringsInWorld();

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SetupSimulation(SimulationSettings settings);

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RunSimulation(int iterations);

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
	    public static extern float GetWorldXmin();

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
	    public static extern float GetWorldXmax();

        private delegate void DelegateDrawFrame();

        List<Node> m_listNodes;
        List<Spring> m_listSprings;
        float m_scale = 1.0f;
        float m_offsetX = -100.0f;
        float m_offsetY = -5.0f;
        float m_worldSize = 200.0f;
        static bool m_isRunning = false;
        int m_cyclesDraw = 10;
        static int m_drawDelay = 10;
        Thread m_simulationThread;
        Image[] m_renderScreen;
        int m_indexBuffer = 0;

        public VisualSimulation()
        {
            InitializeComponent();
            m_renderScreen = new Bitmap[2];
            m_renderScreen[0] = new Bitmap(panelScreen.Width, panelScreen.Height);
            m_renderScreen[1] = new Bitmap(panelScreen.Width, panelScreen.Height);
            SetWorldSize(50.0f);
            DrawFrame();
            panelScreen.Refresh();
        }

        private void buttonOpenResults_Click(object sender, EventArgs e)
        {
            if (openFileDialogResults.ShowDialog() == DialogResult.OK)
            {
                Console.WriteLine("File opened");
            }

            SimulationSettings settings = new SimulationSettings();
            settings.m_dnaFilename = openFileDialogResults.FileName;
            m_listNodes = new List<Node>();
            m_listSprings = new List<Spring>();
            SetupSimulation(settings);

            float xmin = GetWorldXmin();
            float xmax = GetWorldXmax();
            float effectiveWidth = xmax - xmin;

            SetWorldSize(3 * effectiveWidth);
            DrawFrame();
        }

        private void DrawFrame()
        {
            m_indexBuffer = (m_indexBuffer + 1) % 2;
            Graphics g = Graphics.FromImage(m_renderScreen[m_indexBuffer]);
            DrawBackground(g);

            int structNodeSize = Marshal.SizeOf(typeof(Node));
            int numNodes = GetNumberNodesInWorld();

            if (numNodes == 0)
            {
                panelScreen.Refresh();
                return;
            }

            float xmin = GetWorldXmin();
            float xmax = GetWorldXmax();
            float effectiveWidth = xmax - xmin;

            m_offsetX = -(m_worldSize - effectiveWidth) * 0.5f;

            IntPtr nodesList = GetNodesInWorld();
            if (nodesList == null)
            {
                Console.WriteLine("ERROR: nodesList == null");
                return;
            }

            SolidBrush redBrush = new SolidBrush(Color.Red);
            for (int i = 0; i < numNodes; ++i)
            {
                IntPtr data = new IntPtr(nodesList.ToInt64() + structNodeSize * i);
                if (data == null)
                {
                    Console.WriteLine("ERROR: data == null");
                    continue;
                }

                Node n = (Node)Marshal.PtrToStructure(data, typeof(Node));
                m_listNodes.Add(n);

                float radius = n.m_radius;
                Point pos = WorldToScreen(n.m_posX - radius, n.m_posY + radius);
                float size = radius * 2 * m_scale;
                g.FillEllipse(redBrush, pos.X, pos.Y, size, size);
            }

            int structSpringSize = Marshal.SizeOf(typeof(Spring));

            int numSprings = GetNumberSpringsInWorld();
            IntPtr springsList = GetSpringsInWorld();
            if (springsList == null)
            {
                Console.WriteLine("ERROR: springList == null");
                return;
            }

            Pen pen = new Pen(Color.Yellow, 2.0f);
            for (int i = 0; i < numSprings; ++i)
            {
                IntPtr data = new IntPtr(springsList.ToInt64() + structSpringSize * i);

                Spring s = (Spring)Marshal.PtrToStructure(data, typeof(Spring));
                m_listSprings.Add(s);
                Point pt1 = WorldToScreen(s.m_p1X, s.m_p1Y);
                Point pt2 = WorldToScreen(s.m_p2X, s.m_p2Y);
                g.DrawLine(pen, pt1, pt2);
            }

            panelScreen.Refresh();
        }

        private void panelScreen_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.TextRenderingHint = TextRenderingHint.AntiAliasGridFit;
            g.InterpolationMode = InterpolationMode.High;
            g.DrawImage(m_renderScreen[m_indexBuffer], new PointF(0.0f, 0.0f));
            return;
        }

        void DrawBackground(Graphics g)
        {
            SolidBrush blackBrush = new SolidBrush(Color.Black);
            g.FillRectangle(blackBrush, 0, 0, panelScreen.Width, panelScreen.Height);

            Pen pen = new Pen(Color.White);

            const int numLines = 10;
            float delta = m_worldSize / (numLines - 1);

            float start = ((int)(m_offsetX / delta)) * delta;
            for (int i = 0; i < numLines; i++)
            {
                Point pt1 = WorldToScreen(start + i * delta, 0.0f);
                Point pt2 = WorldToScreen(start + i * delta, m_worldSize);
                g.DrawLine(pen, pt1, pt2);
                //e.Graphics.DrawString(
            }

            Point pGround = WorldToScreen(0.0f, 0.0f);
            g.DrawLine(pen, 0, pGround.Y, panelScreen.Width, pGround.Y);
        }

        Point WorldToScreen(float x, float y)
        {
            Point p = new Point();
            p.X = (int)((x - m_offsetX) * m_scale);
            p.Y = panelScreen.Height - (int)((y - m_offsetY) * m_scale);
            return p;
        }

        void SetWorldSize(float size)
        {
            m_worldSize = size;
            m_scale = panelScreen.Width / m_worldSize;
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            m_offsetX = (float)numericUpDown1.Value;
            panelScreen.Refresh();
        }

        private void buttonPlayStop_Click(object sender, EventArgs e)
        {
            if (m_isRunning)
            {
                Console.WriteLine("Stop Simulation");
                buttonPlayStop.Text = "Play";
                m_isRunning = false;
            }
            else
            {
                Console.WriteLine("Start Simulation");
                buttonPlayStop.Text = "Stop";
                m_isRunning = true;
                m_simulationThread = new Thread(() => SimulationThreadEntryPoint(m_cyclesDraw, this, DrawFrame));
                m_simulationThread.Start();
            }

        }
        private static void SimulationThreadEntryPoint(int cyclesDraw, VisualSimulation form, DelegateDrawFrame drawFrame)
        {
            while (m_isRunning)
            {
                RunSimulation(cyclesDraw);
                form.Invoke(new DelegateDrawFrame(drawFrame));
                Thread.Sleep(m_drawDelay);
            } 
        }
    }
}
