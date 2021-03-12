// DONT PUT IN EDITOR FOLDER

///From deebrol https://gist.github.com/deebrol/02f61b7611fd4eca923776077b92dfc2

using System;

using UnityEngine;

/// <summary>
/// Attribute used to show or hide the Field depending on certain conditions
/// </summary>
[AttributeUsage(AttributeTargets.Field, AllowMultiple = false, Inherited = true)]
public class ShowWhenAttribute : PropertyAttribute
{

	public readonly string condition_field_name;
	public readonly object comparation_value;
	public readonly object[] comparation_value_array;

	/// <summary>
	/// Attribute used to show or hide the Field depending on certain conditions
	/// </summary>
	/// <param name="conditionFieldName">Name of the bool condition Field</param>
	public ShowWhenAttribute(string conditionFieldName)
	{
		this.condition_field_name = conditionFieldName;
	}

	/// <summary>
	/// Attribute used to show or hide the Field depending on certain conditions
	/// </summary>
	/// <param name="conditionFieldName">Name of the Field to compare (bool, enum, int or float)</param>
	/// <param name="comparationValue">Value to compare</param>
	public ShowWhenAttribute(string conditionFieldName, object comparationValue = null)
	{
		this.condition_field_name = conditionFieldName;
		this.comparation_value = comparationValue;
	}

	/// <summary>
	/// Attribute used to show or hide the Field depending on certain conditions
	/// </summary>
	/// <param name="conditionFieldName">Name of the Field to compare (bool, enum, int or float)</param>
	/// <param name="comparationValueArray">Array of values to compare (only for enums)</param>
	public ShowWhenAttribute(string conditionFieldName, object[] comparationValueArray = null)
	{
		this.condition_field_name = conditionFieldName;
		this.comparation_value_array = comparationValueArray;
	}
}