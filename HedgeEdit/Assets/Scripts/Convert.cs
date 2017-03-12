﻿using UnityEngine;

public static class Convert
{
    //Methods
    public static Vector3 ToUnity(HedgeLib.Vector3 vect)
    {
        return new Vector3(vect.Z, vect.Y, vect.X);
    }

    public static Vector4 ToUnity(HedgeLib.Vector4 vect)
    {
        return new Vector4(vect.Z, vect.Y, vect.X, vect.W);
    }

    public static Quaternion ToUnity(HedgeLib.Quaternion quat)
    {
        return new Quaternion(-quat.Z, -quat.Y, -quat.X, quat.W);
    }

    public static Light ToUnity(HedgeLib.Lights.Light light)
    {
        GameObject obj = new GameObject();
        var unityLight = obj.AddComponent<Light>();
        var unityPos = ToUnity(light.Position);

        unityLight.color = new Color(light.Color.X, light.Color.Y, light.Color.Z);
        unityLight.shadows = LightShadows.Soft;
        unityLight.type = (light.LightType == HedgeLib.Lights.Light.LightTypes.Directional) ?
            LightType.Directional : LightType.Point;

        if (unityLight.type == LightType.Directional)
            unityLight.transform.LookAt(unityPos);
        else if (unityLight.type == LightType.Point)
            unityLight.transform.position = unityPos;

        return unityLight;
    }
}