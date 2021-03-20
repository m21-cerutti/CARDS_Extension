// PUT IN EDITOR FOLDER

///From deebrol https://gist.github.com/deebrol/02f61b7611fd4eca923776077b92dfc2

using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

using UnityEditor;

using UnityEngine;

[CustomPropertyDrawer(typeof(ShowWhenAttribute))]
public class ShowWhenDrawer : PropertyDrawer
{
	private bool _show_field = true;

	public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
	{
		ShowWhenAttribute attribute = (ShowWhenAttribute)this.attribute;
		SerializedProperty conditionField = property.serializedObject.FindProperty(attribute.condition_field_name);

		// We check that exist a Field with the parameter name
		if(conditionField == null)
		{
			ShowError(position, label, "Error getting the condition Field. Check the name.");
			return;
		}

		switch(conditionField.propertyType)
		{
			case SerializedPropertyType.Boolean:
				try
				{
					bool comparationValue = attribute.comparation_value == null || (bool)attribute.comparation_value;
					_show_field = conditionField.boolValue == comparationValue;
				}
				catch
				{
					ShowError(position, label, "Invalid comparation Value Type");
					return;
				}
				break;
			case SerializedPropertyType.Enum:
				object paramEnum = attribute.comparation_value;
				object[] paramEnumArray = attribute.comparation_value_array;

				if(paramEnum == null && paramEnumArray == null)
				{
					ShowError(position, label, "The comparation enum value is null");
					return;
				}
				else if(IsEnum(paramEnum))
				{
					if(!CheckSameEnumType(new[] { paramEnum.GetType() }, property.serializedObject.targetObject.GetType(), conditionField.propertyPath))
					{
						ShowError(position, label, "Enum Types doesn't match");
						return;
					}
					else
					{
						string enumValue = Enum.GetValues(paramEnum.GetType()).GetValue(conditionField.enumValueIndex).ToString();
						if(paramEnum.ToString() != enumValue)
						{
							_show_field = false;
						}
						else
						{
							_show_field = true;
						}
					}
				}
				else if(IsEnum(paramEnumArray))
				{
					if(!CheckSameEnumType(paramEnumArray.Select(x => x.GetType()), property.serializedObject.targetObject.GetType(), conditionField.propertyPath))
					{
						ShowError(position, label, "Enum Types doesn't match");
						return;
					}
					else
					{
						string enumValue = Enum.GetValues(paramEnumArray[0].GetType()).GetValue(conditionField.enumValueIndex).ToString();
						if(paramEnumArray.All(x => x.ToString() != enumValue))
						{
							_show_field = false;
						}
						else
						{
							_show_field = true;
						}
					}
				}
				else
				{
					ShowError(position, label, "The comparation enum value is not an enum");
					return;
				}
				break;
			case SerializedPropertyType.Integer:
			case SerializedPropertyType.Float:
				string stringValue;
				bool error = false;

				float conditionValue = 0;
				if(conditionField.propertyType == SerializedPropertyType.Integer)
				{
					conditionValue = conditionField.intValue;
				}
				else if(conditionField.propertyType == SerializedPropertyType.Float)
				{
					conditionValue = conditionField.floatValue;
				}

				try
				{
					stringValue = (string)attribute.comparation_value;
				}
				catch
				{
					ShowError(position, label, "Invalid comparation Value Type");
					return;
				}

				if(stringValue.StartsWith("=="))
				{
					float? value = GetValue(stringValue, "==");
					if(value == null)
					{
						error = true;
					}
					else
					{
						_show_field = conditionValue == value;
					}
				}
				else if(stringValue.StartsWith("!="))
				{
					float? value = GetValue(stringValue, "!=");
					if(value == null)
					{
						error = true;
					}
					else
					{
						_show_field = conditionValue != value;
					}
				}
				else if(stringValue.StartsWith("<="))
				{
					float? value = GetValue(stringValue, "<=");
					if(value == null)
					{
						error = true;
					}
					else
					{
						_show_field = conditionValue <= value;
					}
				}
				else if(stringValue.StartsWith(">="))
				{
					float? value = GetValue(stringValue, ">=");
					if(value == null)
					{
						error = true;
					}
					else
					{
						_show_field = conditionValue >= value;
					}
				}
				else if(stringValue.StartsWith("<"))
				{
					float? value = GetValue(stringValue, "<");
					if(value == null)
					{
						error = true;
					}
					else
					{
						_show_field = conditionValue < value;
					}
				}
				else if(stringValue.StartsWith(">"))
				{
					float? value = GetValue(stringValue, ">");
					if(value == null)
					{
						error = true;
					}
					else
					{
						_show_field = conditionValue > value;
					}
				}

				if(error)
				{
					ShowError(position, label, "Invalid comparation instruction for Int or float value");
					return;
				}
				break;
			default:
				ShowError(position, label, "This type has not supported.");
				return;
		}

		if(_show_field)
		{
			EditorGUI.PropertyField(position, property, true);
		}
	}

	public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
	{
		if(_show_field)
		{
			return EditorGUI.GetPropertyHeight(property);
		}
		else
		{
			return -EditorGUIUtility.standardVerticalSpacing;
		}
	}

	/// <summary>
	/// Return if the object is enum and not null
	/// </summary>
	private static bool IsEnum(object obj)
	{
		return obj != null && obj.GetType().IsEnum;
	}

	/// <summary>
	/// Return if all the objects are enums and not null
	/// </summary>
	private static bool IsEnum(object[] obj)
	{
		return obj != null && obj.All(o => o.GetType().IsEnum);
	}

	/// <summary>
	/// Check if the field with name "fieldName" has the same class as the "checkTypes" classes through reflection
	/// </summary>
	private static bool CheckSameEnumType(IEnumerable<Type> checkTypes, Type classType, string fieldName)
	{
		FieldInfo memberInfo;
		string[] fields = fieldName.Split('.');
		if(fields.Length > 1)
		{
			memberInfo = classType.GetField(fields[0]);
			for(int i = 1; i < fields.Length; i++)
			{
				memberInfo = memberInfo.FieldType.GetField(fields[i]);
			}
		}
		else
		{
			memberInfo = classType.GetField(fieldName);
		}

		if(memberInfo != null)
		{
			return checkTypes.All(x => x == memberInfo.FieldType);
		}

		return false;
	}

	private void ShowError(Rect position, GUIContent label, string errorText)
	{
		EditorGUI.LabelField(position, label, new GUIContent(errorText));
		_show_field = true;
	}

	/// <summary>
	/// Return the float value in the content string removing the remove string
	/// </summary>
	private static float? GetValue(string content, string remove)
	{
		string removed = content.Replace(remove, "");
		try
		{
			return float.Parse(removed);
		}
		catch
		{
			return null;
		}
	}
}