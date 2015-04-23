namespace fnt2myspr
{
	partial class FormMain
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
			this.richTextBoxFnt = new System.Windows.Forms.RichTextBox();
			this.richTextBoxMyspr = new System.Windows.Forms.RichTextBox();
			this.buttonConvert = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// richTextBoxFnt
			// 
			this.richTextBoxFnt.Location = new System.Drawing.Point(12, 12);
			this.richTextBoxFnt.Name = "richTextBoxFnt";
			this.richTextBoxFnt.Size = new System.Drawing.Size(356, 462);
			this.richTextBoxFnt.TabIndex = 0;
			this.richTextBoxFnt.Text = "";
			this.richTextBoxFnt.WordWrap = false;
			// 
			// richTextBoxMyspr
			// 
			this.richTextBoxMyspr.Location = new System.Drawing.Point(426, 12);
			this.richTextBoxMyspr.Name = "richTextBoxMyspr";
			this.richTextBoxMyspr.Size = new System.Drawing.Size(356, 462);
			this.richTextBoxMyspr.TabIndex = 1;
			this.richTextBoxMyspr.Text = "";
			this.richTextBoxMyspr.WordWrap = false;
			// 
			// buttonConvert
			// 
			this.buttonConvert.Location = new System.Drawing.Point(374, 222);
			this.buttonConvert.Name = "buttonConvert";
			this.buttonConvert.Size = new System.Drawing.Size(46, 43);
			this.buttonConvert.TabIndex = 2;
			this.buttonConvert.Text = ">>";
			this.buttonConvert.UseVisualStyleBackColor = true;
			this.buttonConvert.Click += new System.EventHandler(this.buttonConvert_Click);
			// 
			// FormMain
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(794, 486);
			this.Controls.Add(this.buttonConvert);
			this.Controls.Add(this.richTextBoxMyspr);
			this.Controls.Add(this.richTextBoxFnt);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.MaximizeBox = false;
			this.Name = "FormMain";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "fnt2myspr";
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.RichTextBox richTextBoxFnt;
		private System.Windows.Forms.RichTextBox richTextBoxMyspr;
		private System.Windows.Forms.Button buttonConvert;
	}
}

