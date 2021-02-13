// IORP Script
// #include <izcmd>
#define	BasicWorld		10002 //�������ֺ�  
// 5666~6666���������� 10001��DM

enum pworldtype {
    w_invite
};

new pWorld[MAX_PLAYERS][pworldtype];

stock Initialize_pWorld(const playerid) {
    pWorld[playerid][w_invite] = -1;
}

stock MyWorld(const playerid) {
    return playerid + BasicWorld;
}

CMD:w(playerid, cmdtext[]) { //�л�����
    new worlds, str[128];
    if(sscanf(cmdtext, "i", worlds)) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), 0);
        SetPlayerVirtualWorld(playerid, 0);
        SCM(playerid, Color_WorldRed, "[����] ��ص��˴�����.");
        return 1;
    }
    if(worlds < 0 || worlds > 10000 || worlds >= 5666 && worlds <= 6666) return SCM(playerid, Color_WorldRed, "[����] /w 0Ϊ�����磬����ֻ���л�Ϊ0-10000���Ҳ��ɽ���5666-6666����������");
    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), worlds);
    SetPlayerVirtualWorld(playerid, worlds);
    format(str, sizeof(str), "[����] ���л��� %d ��ƽ������.", worlds);
    SCM(playerid, Color_WorldRed, str);
    for (new i = GetPlayerPoolSize(); i >= 0; i--) {
        if(IsPlayerConnected(i)) {
            if(PlayerInfo[i][tvid] == playerid && i != playerid) {
                SetPlayerVirtualWorld(i, GetPlayerVirtualWorld(playerid));
                SetPlayerInterior(i, GetPlayerInterior(playerid));
                if(IsPlayerInAnyVehicle(i)) PlayerSpectateVehicle(i, GetPlayerVehicleID(playerid));
                else PlayerSpectatePlayer(i, playerid);
                SCM(i, Color_Orange, "[TV]:�Է��л������磬�Զ�׷�ٹۿ�.");
            }
        }
    }
    return 1;
}
CMD:ww(playerid, cmdtext[]) {
    new tmp[128], idx;
    tmp = strtok(cmdtext, idx);
    if(!strlen(tmp)) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), MyWorld(playerid));
        SetPlayerVirtualWorld(playerid, MyWorld(playerid));
        SCM(playerid, Color_WorldRed, "[����] ����������ר������.");
        SCM(playerid, Color_WorldRed, "[����] /ww kick - �߳�����, /ww invite - �������, /ww back - �ص�������.");
        for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            if(IsPlayerConnected(i)) {
                if(PlayerInfo[i][tvid] == playerid && i != playerid) {
                    SetPlayerVirtualWorld(i, GetPlayerVirtualWorld(playerid));
                    SetPlayerInterior(i, GetPlayerInterior(playerid));
                    if(IsPlayerInAnyVehicle(i)) PlayerSpectateVehicle(i, GetPlayerVehicleID(playerid));
                    else PlayerSpectatePlayer(i, playerid);
                    SCM(i, Color_Orange, "[TV]:�Է��л������磬�Զ�׷�ٹۿ�.");
                }
            }
        }
        return 1;
    }
    if(mk_strcmp(tmp, "back", true) == 0) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), 0);
        SetPlayerVirtualWorld(playerid, 0);
        SCM(playerid, Color_WorldRed, "[����] ��ص��˴�����.");
        return 1;
    }
    if(mk_strcmp(tmp, "yes", true) == 0) {
        if(pWorld[playerid][w_invite] == -1) return SCM(playerid, Color_WorldRed, "[����] Ŀǰ����������Ŷ.");
        if(IsPlayerConnected(pWorld[playerid][w_invite]) == 0) return SCM(playerid, Color_WorldRed, "[����] �Է�������.");
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), pWorld[playerid][w_invite] + BasicWorld);
        SetPlayerVirtualWorld(playerid, pWorld[playerid][w_invite] + BasicWorld);
        format(tmp, sizeof tmp, "[����] ������� %s ��ר������.", GetName(pWorld[playerid][w_invite]));
        SCM(playerid, Color_WorldRed, tmp);
        format(tmp, sizeof tmp, "[����] %s ���������ר������.", GetName(playerid));
        SCM(pWorld[playerid][w_invite], Color_WorldRed, tmp);
        pWorld[playerid][w_invite] = -1;
        for (new i = GetPlayerPoolSize(); i >= 0; i--) {
            if(IsPlayerConnected(i)) {
                if(PlayerInfo[i][tvid] == playerid && i != playerid) {
                    SetPlayerVirtualWorld(i, GetPlayerVirtualWorld(playerid));
                    SetPlayerInterior(i, GetPlayerInterior(playerid));
                    if(IsPlayerInAnyVehicle(i)) PlayerSpectateVehicle(i, GetPlayerVehicleID(playerid));
                    else PlayerSpectatePlayer(i, playerid);
                    SCM(i, Color_Orange, "[TV]:�Է��л������磬�Զ�׷�ٹۿ�.");
                }
            }
        }
        return 1;
    }
    if(mk_strcmp(tmp, "invite", true) == 0) {
        tmp = strtok(cmdtext, idx);
        if(!strlen(tmp)) return SCM(playerid, Color_WorldRed, "[����] /ww invite [�Է�ID] (����ĳ�˽����ҵ�ר������)");
        new id = strval(tmp);
        if(IsPlayerConnected(id) == 0 || id==playerid || IsPlayerNPC(id)) return SCM(playerid, Color_WorldRed, "[����] ������ĶԷ�ID����.");
        if(GetPlayerVirtualWorld(id) == MyWorld(playerid)) return SCM(playerid, Color_WorldRed, "[����] ���Ѿ������ר����������.");
        format(tmp, sizeof tmp, "[����] �ѽ����뷢�� %s �����ظ�.", GetName(id));
        SCM(playerid, Color_WorldRed, tmp);
        format(tmp, sizeof tmp, "[����] %s �������������ר������, ͬ������'/ww yes', ��ͬ�������.", GetName(playerid));
        SCM(id, Color_WorldRed, tmp);
        pWorld[id][w_invite] = playerid;
        return 1;
    }
    if(mk_strcmp(tmp, "kick", true) == 0) {
        tmp = strtok(cmdtext, idx);
        if(!strlen(tmp)) return SCM(playerid, Color_WorldRed, "[����] /ww kick [�Է�ID] (��ĳ���߳��ҵ�ר������)");
        new id = strval(tmp);
        if(IsPlayerConnected(id) == 0 || id==playerid|| IsPlayerNPC(id)) return SCM(playerid, Color_WorldRed, "[����] ������ĶԷ�ID����.");
        if(GetPlayerVirtualWorld(id) != MyWorld(playerid)) {
            format(tmp, sizeof tmp, "[����] %s ��û�������ר��������.", GetName(id));
            SCM(playerid, Color_WorldRed, tmp);
            return 1;
        }
        SetPlayerVirtualWorld(id, 0);
        format(tmp, sizeof tmp, "[����] ��� %s �߳������ר������.", GetName(id));
        SCM(playerid, Color_WorldRed, tmp);
        format(tmp, sizeof tmp, "[����] �㱻 %s �߳�������ר������", GetName(playerid));
        SCM(id, Color_WorldRed, tmp);
        return 1;
    }
    return 1;
}