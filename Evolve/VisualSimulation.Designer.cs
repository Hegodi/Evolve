
namespace Evolve
{
    partial class VisualSimulation
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonOpenResults = new System.Windows.Forms.Button();
            this.openFileDialogResults = new System.Windows.Forms.OpenFileDialog();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.buttonPlayStop = new System.Windows.Forms.Button();
            this.buttonReset = new System.Windows.Forms.Button();
            this.panelScreen = new System.Windows.Forms.Panel();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonOpenResults
            // 
            this.buttonOpenResults.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonOpenResults.Location = new System.Drawing.Point(3, 3);
            this.buttonOpenResults.Name = "buttonOpenResults";
            this.buttonOpenResults.Size = new System.Drawing.Size(114, 23);
            this.buttonOpenResults.TabIndex = 0;
            this.buttonOpenResults.Text = "Open Results";
            this.buttonOpenResults.UseVisualStyleBackColor = true;
            this.buttonOpenResults.Click += new System.EventHandler(this.buttonOpenResults_Click);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.panelScreen, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(790, 502);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 4;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 120F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel2.Controls.Add(this.buttonOpenResults, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.buttonPlayStop, 2, 0);
            this.tableLayoutPanel2.Controls.Add(this.buttonReset, 3, 0);
            this.tableLayoutPanel2.Controls.Add(this.numericUpDown1, 1, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 470);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(784, 29);
            this.tableLayoutPanel2.TabIndex = 0;
            // 
            // buttonPlayStop
            // 
            this.buttonPlayStop.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonPlayStop.Location = new System.Drawing.Point(667, 3);
            this.buttonPlayStop.Name = "buttonPlayStop";
            this.buttonPlayStop.Size = new System.Drawing.Size(54, 23);
            this.buttonPlayStop.TabIndex = 1;
            this.buttonPlayStop.Text = "Play";
            this.buttonPlayStop.UseVisualStyleBackColor = true;
            this.buttonPlayStop.Click += new System.EventHandler(this.buttonPlayStop_Click);
            // 
            // buttonReset
            // 
            this.buttonReset.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonReset.Location = new System.Drawing.Point(727, 3);
            this.buttonReset.Name = "buttonReset";
            this.buttonReset.Size = new System.Drawing.Size(54, 23);
            this.buttonReset.TabIndex = 2;
            this.buttonReset.Text = "Reset";
            this.buttonReset.UseVisualStyleBackColor = true;
            // 
            // panelScreen
            // 
            this.panelScreen.BackColor = System.Drawing.Color.Black;
            this.panelScreen.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelScreen.Location = new System.Drawing.Point(3, 3);
            this.panelScreen.Name = "panelScreen";
            this.panelScreen.Size = new System.Drawing.Size(784, 461);
            this.panelScreen.TabIndex = 1;
            this.panelScreen.Paint += new System.Windows.Forms.PaintEventHandler(this.panelScreen_Paint);
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.numericUpDown1.Location = new System.Drawing.Point(123, 3);
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(120, 20);
            this.numericUpDown1.TabIndex = 3;
            this.numericUpDown1.ValueChanged += new System.EventHandler(this.numericUpDown1_ValueChanged);
            // 
            // VisualSimulation
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(790, 502);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "VisualSimulation";
            this.Text = "Evolve - Visualization";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonOpenResults;
        private System.Windows.Forms.OpenFileDialog openFileDialogResults;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Panel panelScreen;
        private System.Windows.Forms.Button buttonPlayStop;
        private System.Windows.Forms.Button buttonReset;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
    }
}