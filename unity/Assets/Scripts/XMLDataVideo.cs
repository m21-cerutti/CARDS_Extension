/// Generated with http://xmltocsharp.azurewebsites.net/

using System;
using System.Collections.Generic;
using System.IO;
using System.Xml.Serialization;

/// <summary>
/// Permit to read XML video annotations and get data structure.
/// Watchout, all value are string and need to be converted.
/// Use with XMLVideoAnotations.Load(path);
/// </summary>
namespace Xml2CSharp
{
	[XmlRoot(ElementName = "XMLTarget")]
	public class XMLTarget
	{
		[XmlElement(ElementName = "id")]
		public string Id { get; set; }
		[XmlElement(ElementName = "state")]
		public string State { get; set; }
		[XmlElement(ElementName = "x")]
		public string X { get; set; }
		[XmlElement(ElementName = "y")]
		public string Y { get; set; }
		[XmlElement(ElementName = "width")]
		public string Width { get; set; }
		[XmlElement(ElementName = "height")]
		public string Height { get; set; }
	}

	[XmlRoot(ElementName = "ListTargets")]
	public class ListTargets
	{
		[XmlElement(ElementName = "XMLTarget")]
		public List<XMLTarget> Targets { get; set; }
	}

	[XmlRoot(ElementName = "Frame")]
	public class XMLFrame
	{
		[XmlElement(ElementName = "number")]
		public string Number { get; set; }
		[XmlElement(ElementName = "ListTargets")]
		public ListTargets ListTargets { get; set; }
	}

	[XmlRoot(ElementName = "Frames")]
	public class ListFrames
	{
		[XmlElement(ElementName = "Frame")]
		public List<XMLFrame> Frames { get; set; }
	}

	[XmlRoot(ElementName = "opencv_storage")]
	public class OpencvStorage
	{
		[XmlElement(ElementName = "Frames")]
		public ListFrames ListFrames { get; set; }

		public static OpencvStorage Load(string path)
		{
			try
			{
				XmlSerializer serializer = new XmlSerializer(typeof(OpencvStorage));
				using(FileStream stream = new FileStream(path, FileMode.Open))
				{
					return serializer.Deserialize(stream) as OpencvStorage;
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
