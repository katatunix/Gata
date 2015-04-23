using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace fnt2myspr
{
	public partial class FormMain : Form
	{
		public FormMain()
		{
			InitializeComponent();
		}

		private void buttonConvert_Click(object sender, EventArgs e)
		{
			XmlDocument doc = new XmlDocument();
			doc.LoadXml(richTextBoxFnt.Text);

			string res = "<?xml version=\"1.0\" ?>\n\n";
			res += "<sprite>\n\n";

			// module
			XmlNodeList charNodeList = doc.GetElementsByTagName("char");
			for (int i = 0; i < charNodeList.Count; i++)
			{
				string x = charNodeList[i].Attributes["x"].Value;
				string y = charNodeList[i].Attributes["y"].Value;
				string w = charNodeList[i].Attributes["width"].Value;
				string h = charNodeList[i].Attributes["height"].Value;
				res += "<module x=\"" + x + "\" y=\"" + y + "\" w=\"" + w + "\" h=\"" + h + "\" />\n";
			}

			// frame
			res += "\n\n<frames>\n";
			res += "\t<frame>\n";
			for (int i = 0; i < charNodeList.Count; i++)
			{
				string xInFrame = charNodeList[i].Attributes["xoffset"].Value;
				string yInFrame = charNodeList[i].Attributes["yoffset"].Value;
				res += "\t\t<fmodule moduleIndex=\"" + i + "\" xInFrame=\"" + xInFrame + "\" yInFrame=\"" + yInFrame + "\">";
				res += "</fmodule>\n";
			}
			res += "\t</frame>\n";
			res += "</frames>\n";

			// charmap
			res += "\n\n<charmap charSpace=\"2\" lineSpace=\"10\" spaceWidth=\"15\">\n";

			for (int i = 0; i < charNodeList.Count; i++)
			{
				string id = charNodeList[i].Attributes["id"].Value;
				res += "\t<char>" + id + "</char>\n";
			}

			res += "</charmap>\n";

			//
			res += "\n\n</sprite>";

			richTextBoxMyspr.Text = res;
		}
	}
}
