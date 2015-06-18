/*
	Attachments Fix Include (attachments.inc)
	* This include fixes the attachments bug, which is expierenced while zooming with snipers, camera, RPGs..
	* The attached object info now can be retrieved from GetPlayerAttachedObject
	* This include also restores player attachments on Spawn, so they will remain!
	* Disconnect fix; Destroys the attachments on Disconnect.

 	Author: (creator)
	* Gammix

	(c) Copyright 2015
  	* This file is provided as is (no warranties).
*/

/*
FUNCTIONS:
native SetPlayerAttachedObject(playerid, index, modelid, bone, Float:fOffsetX = 0.0, Float:fOffsetY = 0.0, Float:fOffsetZ = 0.0, Float:fRotX = 0.0, Float:fRotY = 0.0, Float:fRotZ = 0.0, Float:fScaleX = 1.0, Float:fScaleY = 1.0, Float:fScaleZ = 1.0, materialcolor1 = 0, materialcolor2 = 0);
native GetPlayerAttachedObject(playerid, index, &modelid, &bone, &Float:fOffsetX, &Float:fOffsetY, &Float:fOffsetZ, &Float:fRotX, &Float:fRotY, &Float:fRotZ, &Float:fScaleX, &Float:fScaleY, &Float:fScaleZ, &materialcolor1, &materialcolor2);
native RemovePlayerAttachedObject(playerid, index);
*/

enum AttachmentData
{
	att_modelid,
	att_bone,
	Float:att_offset_x,
	Float:att_offset_y,
	Float:att_offset_z,
	Float:att_rot_x,
	Float:att_rot_y,
	Float:att_rot_z,
	Float:att_scale_x,
	Float:att_scale_y,
	Float:att_scale_z,
	att_color_1,
	att_color_2,
	bool:att_used
};
static gAttachment[MAX_PLAYERS][MAX_PLAYER_ATTACHED_OBJECTS][AttachmentData];

public OnPlayerSpawn(playerid)
{
	for(new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
	{
	    if(gAttachment[playerid][i][att_used])
	    {
	        SetPlayerAttachedObject(	playerid,
										i,
										gAttachment[playerid][i][att_modelid],
										gAttachment[playerid][i][att_bone],
										gAttachment[playerid][i][att_offset_x],
										gAttachment[playerid][i][att_offset_y],
										gAttachment[playerid][i][att_offset_z],
										gAttachment[playerid][i][att_rot_x],
										gAttachment[playerid][i][att_rot_y],
										gAttachment[playerid][i][att_rot_z],
										gAttachment[playerid][i][att_scale_x],
										gAttachment[playerid][i][att_scale_y],
										gAttachment[playerid][i][att_scale_z],
										gAttachment[playerid][i][att_color_1],
										gAttachment[playerid][i][att_color_2]
									);
	    }
	}

    #if defined ATT_OnPlayerSpawn
       	return ATT_OnPlayerSpawn(playerid);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn ATT_OnPlayerSpawn
#if defined ATT_OnPlayerSpawn
    forward ATT_OnPlayerSpawn(playerid);
#endif

public OnPlayerConnect(playerid)
{
	for(new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
	{
	    gAttachment[playerid][i][att_used] = false;
	}

    #if defined ATT_OnPlayerConnect
       	return ATT_OnPlayerConnect(playerid);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect ATT_OnPlayerConnect
#if defined ATT_OnPlayerConnect
    forward ATT_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
	for(new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
	{
	    if(IsPlayerAttachedObjectSlotUsed(playerid, i))
	    {
	        RemovePlayerAttachedObject(playerid, i);
	        gAttachment[playerid][i][att_used] = false;
	    }
	}

    #if defined ATT_OnPlayerDisconnect
       	return ATT_OnPlayerDisconnect(playerid, reason);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect ATT_OnPlayerDisconnect
#if defined ATT_OnPlayerDisconnect
    forward ATT_OnPlayerDisconnect(playerid, reason);
#endif

#if ! defined KEY_AIM
	#define KEY_AIM 128
#endif

#if ! defined HOLDING
    #define HOLDING(%0) ((newkeys & (%0)) == (%0))
#endif

#if ! defined RELEASED
	#define RELEASED(%0) (((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))
#endif

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(HOLDING(KEY_AIM))
	{
	    switch(GetPlayerWeapon(playerid))
		{
			case WEAPON_SNIPER, WEAPON_ROCKETLAUNCHER, WEAPON_HEATSEEKER, WEAPON_CAMERA:
			{
			    for(new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
				{
				    if(	IsPlayerAttachedObjectSlotUsed(playerid, i) &&
						gAttachment[playerid][i][att_used])
				    {
						RemovePlayerAttachedObject(playerid, i);
	     			}
		        }
		    }
		}
	}
	if(RELEASED(KEY_AIM))
	{
	    for(new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
		{
		    if(	! IsPlayerAttachedObjectSlotUsed(playerid, i) &&
				gAttachment[playerid][i][att_used])
		    {
		        SetPlayerAttachedObject(	playerid,
											i,
											gAttachment[playerid][i][att_modelid],
											gAttachment[playerid][i][att_bone],
											gAttachment[playerid][i][att_offset_x],
											gAttachment[playerid][i][att_offset_y],
											gAttachment[playerid][i][att_offset_z],
											gAttachment[playerid][i][att_rot_x],
											gAttachment[playerid][i][att_rot_y],
											gAttachment[playerid][i][att_rot_z],
											gAttachment[playerid][i][att_scale_x],
											gAttachment[playerid][i][att_scale_y],
											gAttachment[playerid][i][att_scale_z],
											gAttachment[playerid][i][att_color_1],
											gAttachment[playerid][i][att_color_2]
									);
		    }
		}
	}

	#if defined ATT_OnPlayerKeyStateChange
       	return ATT_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange ATT_OnPlayerKeyStateChange
#if defined ATT_OnPlayerKeyStateChange
    forward ATT_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

stock ATT_SetPlayerAttachedObject(playerid, index, modelid, bone, Float:fOffsetX = 0.0, Float:fOffsetY = 0.0, Float:fOffsetZ = 0.0, Float:fRotX = 0.0, Float:fRotY = 0.0, Float:fRotZ = 0.0, Float:fScaleX = 1.0, Float:fScaleY = 1.0, Float:fScaleZ = 1.0, materialcolor1 = 0, materialcolor2 = 0)
{
	if(SetPlayerAttachedObject(playerid, index, modelid, bone, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ, materialcolor1, materialcolor2))
	{
    	gAttachment[playerid][index][att_used] = true;
    	gAttachment[playerid][index][att_modelid] = modelid;
		gAttachment[playerid][index][att_bone] = bone;
		gAttachment[playerid][index][att_offset_x] = fOffsetX;
		gAttachment[playerid][index][att_offset_y] = fOffsetY;
		gAttachment[playerid][index][att_offset_z] = fOffsetZ;
		gAttachment[playerid][index][att_rot_x] = fRotX;
		gAttachment[playerid][index][att_rot_y] = fRotY;
		gAttachment[playerid][index][att_rot_z] = fRotZ;
		gAttachment[playerid][index][att_scale_x] = fScaleX;
		gAttachment[playerid][index][att_scale_y] = fScaleY;
		gAttachment[playerid][index][att_scale_z] = fScaleZ;
		gAttachment[playerid][index][att_color_1] = materialcolor1;
		gAttachment[playerid][index][att_color_2] = materialcolor2;
		return true;
	}
	return false;
}
#if defined _ALS_SetPlayerAttachedObject
    #undef SetPlayerAttachedObject
#else
    #define _ALS_SetPlayerAttachedObject
#endif
#define SetPlayerAttachedObject ATT_SetPlayerAttachedObject

stock GetPlayerAttachedObject(playerid, index, &modelid, &bone, &Float:fOffsetX, &Float:fOffsetY, &Float:fOffsetZ, &Float:fRotX, &Float:fRotY, &Float:fRotZ, &Float:fScaleX, &Float:fScaleY, &Float:fScaleZ, &materialcolor1, &materialcolor2)
{
    if(gAttachment[playerid][index][att_used])
    {
        modelid = gAttachment[playerid][index][att_modelid];
        bone = gAttachment[playerid][index][att_bone];
        fOffsetX = gAttachment[playerid][index][att_offset_x];
        fOffsetY = gAttachment[playerid][index][att_offset_y];
        fOffsetZ = gAttachment[playerid][index][att_offset_z];
        fRotX = gAttachment[playerid][index][att_rot_x];
        fRotY = gAttachment[playerid][index][att_rot_y];
        fRotZ = gAttachment[playerid][index][att_rot_z];
        fScaleX = gAttachment[playerid][index][att_scale_x];
        fScaleY = gAttachment[playerid][index][att_scale_y];
        fScaleZ = gAttachment[playerid][index][att_scale_z];
        materialcolor1 = gAttachment[playerid][index][att_color_1];
        materialcolor2 = gAttachment[playerid][index][att_color_2];
        return true;
    }
	return false;
}

stock ATT_RemovePlayerAttachedObject(playerid, index)
{
	if(RemovePlayerAttachedObject(playerid, index))
	{
    	gAttachment[playerid][index][att_used] = false;
		return true;
	}
	return false;
}
#if defined _ALS_RemovePlayerAttachedObject
    #undef RemovePlayerAttachedObject
#else
    #define _ALS_RemovePlayerAttachedObject
#endif
#define RemovePlayerAttachedObject ATT_RemovePlayerAttachedObject
