using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;

namespace SpriteDef
{
	class Program
	{
		static void Main(string[] args)
		{
			if (args.Length == 0)
			{
				Console.WriteLine("Usage: SpriteDef.exe <mysprfile> <headerfile>");
				return;
			}

			string mysprfile = args[0];
			string headerfile = args[1];

			Dictionary<int, string> frameDict = new Dictionary<int,string>();
			Dictionary<int, string> animDict = new Dictionary<int, string>();

			XmlDocument doc = new XmlDocument();
			doc.Load(mysprfile);
			XmlNodeList nodeList;
			int count;

			// FRAME
			nodeList = doc.GetElementsByTagName("frame");
			count = 0;
			for (int i = 0; i < nodeList.Count; i++)
			{
				if (nodeList[i].Attributes["name"] != null)
				{
					string name = nodeList[i].Attributes["name"].Value;
					frameDict.Add(count, name);
				}

				if (nodeList[i].Attributes["isset"] != null)
				{
					count += int.Parse(nodeList[i].Attributes["count"].Value);
				}
				else
				{
					count++;
				}
			}

			//
			nodeList = doc.GetElementsByTagName("map_frame");
			for (int i = 0; i < nodeList.Count; i++)
			{
				int index = -1;
				string name = null;
				if (nodeList[i].Attributes["name"] != null && nodeList[i].Attributes["index"] != null)
				{
					try
					{
						index = int.Parse(nodeList[i].Attributes["index"].Value);
					}
					catch (Exception)
					{
					}
					name = nodeList[i].Attributes["name"].Value.Trim();
					if (index > -1 && name.Length > 0)
					{
						frameDict.Remove(index);
						frameDict.Add(index, name);
					}
				}
			}

			// ANIM
			nodeList = doc.GetElementsByTagName("anim");
			count = 0;
			for (int i = 0; i < nodeList.Count; i++)
			{
				if (nodeList[i].Attributes["name"] != null)
				{
					string name = nodeList[i].Attributes["name"].Value;
					animDict.Add(count, name);
				}
				if (nodeList[i].Attributes["isset"] != null)
				{
					count += int.Parse(nodeList[i].Attributes["count"].Value);
				}
				else
				{
					count++;
				}
			}

			nodeList = doc.GetElementsByTagName("map_anim");
			for (int i = 0; i < nodeList.Count; i++)
			{
				int index = -1;
				string name = null;
				if (nodeList[i].Attributes["name"] != null && nodeList[i].Attributes["index"] != null)
				{
					try
					{
						index = int.Parse(nodeList[i].Attributes["index"].Value);
					}
					catch (Exception)
					{
					}
					name = nodeList[i].Attributes["name"].Value.Trim();
					if (index > -1 && name.Length > 0)
					{
						animDict.Remove(index);
						animDict.Add(index, name);
					}
				}
			}
			
			// WRITE
			if (frameDict.Count > 0 || animDict.Count > 0)
			{
				StreamWriter sw = new StreamWriter(headerfile, true);
				sw.WriteLine();
				sw.WriteLine("//////////////////////////// auto-generated");

				if (frameDict.Count > 0)
				{
					sw.WriteLine("///////////// frame");
					foreach (KeyValuePair<int, string> kvp in frameDict)
					{
						sw.WriteLine("#define FRAME_" + kvp.Value + " " + kvp.Key);
					}
				}

				if (animDict.Count > 0)
				{
					sw.WriteLine("///////////// anim");
					foreach (KeyValuePair<int, string> kvp in animDict)
					{
						sw.WriteLine("#define ANIM_" + kvp.Value + " " + kvp.Key);
					}
				}

				sw.Close();
			}
			
			//
			Console.WriteLine("[ SpriteDef ] [ OK ]");
		}
	}
}
