using System;
using System.Runtime.InteropServices;

namespace Evolve
{
    static class EvolveKernelAPI
    {
        public enum EMessageCodes
        {
            EMessageCode_Info,
            EMessageCode_InfoHigh,
            EMessageCode_Warning,
            EMessageCode_Error,
            EMessageCode_Completed
        }

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
            public float m_probInvertCorssover;

            // Evaluation
            public float m_weightDistanceTraveled;
            public float m_weightCollisionsWithGround;
            public float m_weightCollisionsWithObstacles;
            public float m_weightMaxHeight;
            public float m_weightAverageHeight;
            public float m_weightLoadDistanceTraveled;
            public float m_weightLoadCollisionsWithGround;
            public float m_numberNodesGoal;
            public float m_weightNumberNodes;
            public float m_weightReactivity;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct GraphicsSimulationSettings
        {
            public string m_dnaFilename;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct NodeInfo
        {
            public float m_posX;
            public float m_posY;
            public float m_radius;
            public float m_elasticCoefficient;
            public float m_frictionCoefficient;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct SpringInfo
        {
            public int m_indNodeStart;
            public int m_indNodeEnd;
            public float m_length;
            public float m_springConstant;
            public float m_deltaLength;
            public float m_period;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct RobotInfo
        {
            public int m_numNodes;
            public int m_numSprings;
            public IntPtr m_nodes;
            public IntPtr m_springs;
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

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateRobotInfo(string dnaFilename);
        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DisposeRobotInfo(IntPtr robotInfo);

        [DllImport("KernelEvolve.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern float EstimateMemoryNeeded(int populationSize);
    }
}
