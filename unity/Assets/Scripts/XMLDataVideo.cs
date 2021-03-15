using System;
using System.Xml.Serialization;
using System.Collections.Generic;
using System.IO;

namespace Xml2CSharp
{
	[XmlRoot(ElementName = "Object")]
	public class XMLTarget
	{
		[XmlAttribute(AttributeName = "id")]
		public string Id { get; set; }
		[XmlAttribute(AttributeName = "state")]
		public string State { get; set; }
		[XmlAttribute(AttributeName = "x")]
		public string X { get; set; }
		[XmlAttribute(AttributeName = "y")]
		public string Y { get; set; }
		[XmlAttribute(AttributeName = "width")]
		public string Width { get; set; }
		[XmlAttribute(AttributeName = "height")]
		public string Height { get; set; }
	}

	[XmlRoot(ElementName = "frame")]
	public class XMLFrame
	{
		[XmlElement(ElementName = "Object")]
		public List<XMLTarget> Objects { get; set; }
		[XmlAttribute(AttributeName = "number")]
		public string Number { get; set; }
	}

	[XmlRoot(ElementName = "Video_annotations")]
	public class XMLVideoAnotations
	{
		[XmlElement(ElementName = "frame")]
		public List<XMLFrame> Frames { get; set; }
		public static XMLVideoAnotations Load(string path)
		{
			try
			{
				XmlSerializer serializer = new XmlSerializer(typeof(XMLVideoAnotations));
				using(FileStream stream = new FileStream(path, FileMode.Open))
				{
					return serializer.Deserialize(stream) as XMLVideoAnotations;
				}
			}
			catch(Exception e)
			{
				UnityEngine.Debug.LogError("Exception loading config file: " + e);
				return null;
			}
		}
	}
}
