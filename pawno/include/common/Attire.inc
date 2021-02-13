// IORP Script
// #include <a_samp>
/*װ��༭�ű� by Dylan*/
// #include <izcmd>
// #define attire_position					"Attire/Attire.db"
#define MAX_PLAYER_ATTIRE				20 //�������ӵ�е�װ������
// #define Dialog_Attire					400
// #define Dialog_Attire_Select			Dialog_Attire + 1
// #define Dialog_Attire_Bone				Dialog_Attire + 2//��������
// #define Dialog_Attire_SellList			Dialog_Attire + 3//����װ��
// #define Dialog_Attire_Buyit				Dialog_Attire + 4//����װ���ȷ������Ի���
enum attiretype {
    DBID, //���ݿ�id
    UID, //���UID
    Name[MAX_PLAYER_NAME+1], //װ������
    ModelID, //objectģ��id
    Bone, //����id
    Float:OffsetX,
    Float:OffsetY,
    Float:OffsetZ,
    Float:RotX,
    Float:RotY,
    Float:RotZ,
    Float:ScaleX,
    Float:ScaleY,
    Float:ScaleZ,
    Color1, //material��ɫ1id
    Color2, //material��ɫ2id
    State //0 = δװ��, 1 = װ����
}
enum pattiretpye {
    SelectID,
    Slot[MAX_PLAYER_ATTACHED_OBJECTS],
    Page, //����װ���б�ҳ
    PreviewID //����װ���Ԥ��ID
};
new pAttire[MAX_PLAYERS][pattiretpye];
new AttireInfo[MAX_PLAYERS][MAX_PLAYER_ATTIRE][attiretype];
//, DB:attiredb;
new AttireBoneNames[19][12] = {
    " ",
    "��׵",
    "ͷ",
    "���ϱ�",
    "���ϱ�",
    "����",
    "����",
    "�����",
    "�Ҵ���",
    "���",
    "�ҽ�",
    "��С��",
    "��С��",
    "��ǰ��",
    "��ǰ��",
    "������",
    "������",
    "����",
    "�°�"
};
new AttireStateNames[2][MAX_PLAYER_NAME] = {
    "{CFCFCF}δװ��",
    "{00FF7F}װ����"
};
/*----------------------װ����۲���------------------------*/
#define AttireSellTxt			"Attire/Attire.cfg"
#define MAX_ATTIRESELL			500
enum attireselltype {
    Price, //���ۼ۸�
    Name[MAX_PLAYER_NAME], //ģ������
    OBJID, //ģ��id
    preview_bone,
    Float:preview_offsetx,
    Float:preview_offsety,
    Float:preview_offsetz,
    Float:preview_rotx,
    Float:preview_roty,
    Float:preview_rotz,
    Float:preview_scalex,
    Float:preview_scaley,
    Float:preview_scalez
};
new AttireSell[MAX_ATTIRESELL][attireselltype], AttireSellCount, Float:AttireSellPos[3];
stock Initialize_AttireSell() {
    AttireSellCount = 0, AttireSellPos[0] = 0.0, AttireSellPos[1] = 0.0, AttireSellPos[2] = 0.0;
    for (new i = 0; i < MAX_ATTIRESELL; i++) {
        AttireSell[i][OBJID] = 0;
    }
    if(fexist(AttireSellTxt) == 1) {
        new File:m = fopen(AttireSellTxt, io_read), info[256], msg[128], idx = 0;
        while (fread(m, info)) {
            msg = strtok(info, idx);
            if(mk_strcmp(msg, "pos") == 0) {
                AttireSellPos[0] = floatstr(strtok(info, idx));
                AttireSellPos[1] = floatstr(strtok(info, idx));
                AttireSellPos[2] = floatstr(strtok(info, idx));
            }
            if(mk_strcmp(msg, "attire") == 0) {
                new id = -1;
                for (new i = 0; i < MAX_ATTIRESELL; i++) {
                    if(AttireSell[i][OBJID] == 0) {
                        id = i;
                        i = MAX_ATTIRESELL + 1;
                    }
                }
                if(id == -1) {
                    format(msg, sizeof msg, "Attire * ����װ����ص���%d�����ִ���.(�Ѵﵽ����MAX_ATTIRESELL)", AttireSellCount);
                    printf(msg);
                }
                AttireSell[id][Price] = strval(strtok(info, idx));
                format(AttireSell[id][Name], MAX_PLAYER_NAME, strtok(info, idx));
                AttireSell[id][OBJID] = strval(strtok(info, idx));
                AttireSell[id][preview_bone] = strval(strtok(info, idx));
                AttireSell[id][preview_offsetx] = floatstr(strtok(info, idx));
                AttireSell[id][preview_offsety] = floatstr(strtok(info, idx));
                AttireSell[id][preview_offsetz] = floatstr(strtok(info, idx));
                AttireSell[id][preview_rotx] = floatstr(strtok(info, idx));
                AttireSell[id][preview_roty] = floatstr(strtok(info, idx));
                AttireSell[id][preview_rotz] = floatstr(strtok(info, idx));
                AttireSell[id][preview_scalex] = floatstr(strtok(info, idx));
                AttireSell[id][preview_scaley] = floatstr(strtok(info, idx));
                AttireSell[id][preview_scalez] = floatstr(strtok(info, idx));
                AttireSellCount++;
            }
            idx = 0;
        }
        fclose(m);
        printf("Attire * ����װ������ %.3f, %.3f, %.3f", AttireSellPos[0], AttireSellPos[1], AttireSellPos[2]);
        printf("Attire * ������[%d/%d]������װ��", AttireSellCount, MAX_ATTIRESELL);
    } else {
        printf("δ�ҵ������ı����ݶ�ȡʧ��(Attire/Attire.cfg from Attire.inc)");
    }
}
CMD:buyattire(playerid, cmdtext[]) {
    if(GetPlayerDistanceFromPoint(playerid, AttireSellPos[0], AttireSellPos[1], AttireSellPos[2]) > 5.0) {
        SendClientMessage(playerid, Color_Red, "[װ��]�㲻�����¿⹺���.");
        return 1;
    }
    if(pAttire[playerid][PreviewID] == -1) {
        pAttire[playerid][Page] = 1;
        ShowPlayerAttireSellList(playerid, 1);
    } else {
        new msg[128];
        format(msg, sizeof msg, "����:[%s], �۸�:[%d] �Ƿ�ȷ������.", AttireSell[pAttire[playerid][PreviewID]][Name], AttireSell[pAttire[playerid][PreviewID]][Price]);
        Dialog_Show(playerid, Attire_Buyit, DIALOG_STYLE_LIST, "����װ��", msg, "ȷ��", "�ٿ���");
    }
    return 1;
}
stock ShowPlayerAttireSellList(const playerid, const page) {
    new title[128], msg[1024], maxpage; // = GetMaxPage(AttireSellCount);
    if(AttireSellCount % 10 == 0) maxpage = AttireSellCount / 10; //һҳ10��
    else maxpage = (AttireSellCount / 10) + 1;
    format(title, sizeof title, "Attire Sell List (%d/%d)", page, maxpage);
    format(msg, sizeof msg, "��һҳ");
    for (new i = (page - 1) * 10; i < page * 10; i++) {
        if(AttireSell[i][OBJID] != 0) {
            format(msg, 1024, "%s\n %d ����:[%s], �۸�:[%d]", msg, i, AttireSell[i][Name], AttireSell[i][Price]);
        }
    }
    format(msg, sizeof msg, "%s\n��һҳ", msg);
    Dialog_Show(playerid, Attire_SellList, DIALOG_STYLE_LIST, title, msg, "Ԥ��", "�ر�");
}
stock Attire_Preview_Range(const playerid) {
    if(pAttire[playerid][PreviewID] != -1) {
        if(GetPlayerDistanceFromPoint(playerid, AttireSellPos[0], AttireSellPos[1], AttireSellPos[2]) > 15.0) {
            SendClientMessage(playerid, Color_Red, "[װ��]���뿪��Ԥ������.");
            pAttire[playerid][PreviewID] = -1;
            SpawnAttire(playerid);
        }
    }
}
stock Attire_Preview(const playerid, const id) {
    RemovePlayerAttachedObject(playerid, 0);
    SetPlayerAttachedObject(playerid, 0, AttireSell[id][OBJID], AttireSell[id][preview_bone], AttireSell[id][preview_offsetx], AttireSell[id][preview_offsety], AttireSell[id][preview_offsetz],
        AttireSell[id][preview_rotx], AttireSell[id][preview_roty], AttireSell[id][preview_rotz], AttireSell[id][preview_scalex], AttireSell[id][preview_scaley], AttireSell[id][preview_scalez]);
    new msg[128];
    format(msg, sizeof msg, "[װ��]����:[%s], �۸�:[%d], ��������'/buyattire'", AttireSell[id][Name], AttireSell[id][Price]);
    SendClientMessage(playerid, Color_White, msg);
}
stock Attire_Presskey(const playerid, const newkeys) {
    if(newkeys == 8192 && pAttire[playerid][PreviewID] != -1) {
        if(pAttire[playerid][PreviewID] == 0) pAttire[playerid][PreviewID] = AttireSellCount - 1;
        else pAttire[playerid][PreviewID]--;
        Attire_Preview(playerid, pAttire[playerid][PreviewID]);
        return 1;
    }
    if(newkeys == 16384 && pAttire[playerid][PreviewID] != -1) {
        if(pAttire[playerid][PreviewID] >= AttireSellCount - 1) pAttire[playerid][PreviewID] = 0;
        else pAttire[playerid][PreviewID]++;
        Attire_Preview(playerid, pAttire[playerid][PreviewID]);
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------*/
stock Initialize_Attire() //������ʱ��
{
    SetupAttireTable();
    // attiredb = db_open(attire_position);
    Initialize_AttireSell();
    // db_free_result(db_query(attiredb, "PRAGMA synchronous = OFF"));
}
stock Attire_OnGameModeExit() //�ط���ʱ��
{
    // db_close(attiredb);
}
CMD:attire(playerid, cmdtext[]) {
    // if(GetPlayerAccess(GetPlayerAdminProject(playerid), "admin") == 0) return 1;
    // if(GetPlayerAdminLevel(playerid) < 5) return 1;
    if(PlayerInfo[playerid][AdminLevel] < 4) return 0;
    new tmp[128], idx;
    tmp = strtok(cmdtext, idx);
    if(!strlen(tmp)) {
        SendClientMessage(playerid, Color_White, "[װ��]/attire [���ID] [OBJID] [����]");
        return 1;
    }
    new pid = strval(tmp);
    if(!PlayerInfo[pid][Login]) {
        SendClientMessage(playerid, Color_Red, "[װ��]��������ID.");
        return 1;
    }
    tmp = strtok(cmdtext, idx);
    if(!strlen(tmp)) {
        SendClientMessage(playerid, Color_Red, "[װ��]OBJID����Ϊ��.");
        return 1;
    }
    new objid = strval(tmp);
    tmp = strtok(cmdtext, idx);
    if(!strlen(tmp)) {
        SendClientMessage(playerid, Color_Red, "[װ��]���Ʋ���Ϊ��.");
        return 1;
    }
    CreateAttire(PlayerInfo[pid][ID], tmp, objid, 1);
    Attire_ReadData(pid);
    format(tmp, sizeof tmp, "[װ��]OBJID:[%d], ����:[%s], �������:[%s], �����ɹ�.", objid, tmp, GetName(pid));
    SendClientMessage(playerid, Color_White, tmp);
    return 1;
}
stock Attire_Delete(const dbid) {
    new Query[128];
    mysql_format(g_Sql, Query, sizeof Query, "DELETE FROM `attire` WHERE `DBID` = %d", dbid);
    mysql_pquery(g_Sql, Query);
}
stock CreateAttire(const uid, const name[], const modelid, const bone, Float:offsetx = 0.0, Float:offsety = 0.0, \
Float:offsetz = 0.0, Float:rotx = 0.0, Float:roty = 0.0, Float:rotz = 0.0, Float:scalex = 1.1, Float:scaley = 1.1, \
Float:scalez = 1.1, color1 = 0, color2 = 0) {
    new Query[1024];
    mysql_format(g_Sql, Query, sizeof(Query), "INSERT INTO `attire` (`UID`,`Name`,`ModelID`,`Bone`,`OffsetX`,`OffsetY`,`OffsetZ`,`RotX`,\
    `RotY`,`RotZ`,`ScaleX`,`ScaleY`,`ScaleZ`,`Color1`,`Color2`,`State`) VALUES ('%d','%e','%d','%d','%.3f','%.3f','%.3f','%.3f','%.3f',\
    '%.3f','%.3f','%.3f','%.3f','%d','%d',0)",uid, name, modelid, bone, offsetx, offsety, offsetz, rotx, roty, rotz, scalex, \
    scaley, scalez, color1, color2);
    mysql_pquery(g_Sql, Query);
    return 1;
}
stock Initialize_AttireInfo(const playerid) //��ʼ��dbid
{
    for (new i = 0; i < MAX_PLAYER_ATTIRE; i++) {
        AttireInfo[playerid][i][DBID] = 0;
        AttireInfo[playerid][i][State] = 0;
    }
}
stock GetPlayerAlreadyWearCount(const playerid) {
    new count = 0;
    for (new i = 0; i < MAX_PLAYER_ATTIRE; i++) {
        if(AttireInfo[playerid][i][State] == 1) {
            count++;
        }
    }
    return count;
}
stock Initialize_AttireSlot(const playerid) //��ʼ����
{
    for (new i = 0; i < MAX_PLAYER_ATTACHED_OBJECTS; i++) {
        pAttire[playerid][Slot][i] = -1;
    }
}
stock Attire_ReadData(const playerid) //��������ߵ�ʱ���ȡ���ݿ�����
{
    pAttire[playerid][PreviewID] = -1;
    Initialize_AttireInfo(playerid);
    new Query[128];
    mysql_format(g_Sql, Query, sizeof(Query), "SELECT * FROM `attire` WHERE `UID` = %d", PlayerInfo[playerid][ID]);
    mysql_pquery(g_Sql, Query, "AttireReadDataQuery", "d", playerid);
    return 1;
}
function AttireReadDataQuery(playerid) {
    new count = cache_num_rows();
    if(!count) return 1;
    for (new i = 0; i < count; i++) 
    {
        cache_get_value_index_int(i, 0, AttireInfo[playerid][i][DBID]);
        cache_get_value_index(i, 2, AttireInfo[playerid][i][Name]);
        cache_get_value_index_int(i, 3, AttireInfo[playerid][i][ModelID]);
        cache_get_value_index_int(i, 4, AttireInfo[playerid][i][Bone]);
        cache_get_value_index_float(i, 5, AttireInfo[playerid][i][OffsetX]);
        cache_get_value_index_float(i, 6, AttireInfo[playerid][i][OffsetY]);
        cache_get_value_index_float(i, 7, AttireInfo[playerid][i][OffsetZ]);
        cache_get_value_index_float(i, 8, AttireInfo[playerid][i][RotX]);
        cache_get_value_index_float(i, 9, AttireInfo[playerid][i][RotY]);
        cache_get_value_index_float(i, 10, AttireInfo[playerid][i][RotZ]);
        cache_get_value_index_float(i, 11, AttireInfo[playerid][i][ScaleX]);
        cache_get_value_index_float(i, 12, AttireInfo[playerid][i][ScaleY]);
        cache_get_value_index_float(i, 13, AttireInfo[playerid][i][ScaleZ]);
        cache_get_value_index_int(i, 14, AttireInfo[playerid][i][Color1]);
        cache_get_value_index_int(i, 15, AttireInfo[playerid][i][Color2]);
        cache_get_value_index_int(i, 16, AttireInfo[playerid][i][State]);
    }
    return true;
}
stock SpawnAttire(const playerid) //���������ʱ����obj
{
    Initialize_AttireSlot(playerid);
    new slot;
    for (new i = 0; i < MAX_PLAYER_ATTACHED_OBJECTS; i++) {
        if(IsPlayerAttachedObjectSlotUsed(playerid, i) == 1) {
            RemovePlayerAttachedObject(playerid, i);
        }
    }
    for (new i = 0; i < MAX_PLAYER_ATTIRE; i++) {
        if(AttireInfo[playerid][i][State] == 1) {
            slot = GetSlot(playerid);
            if(slot == -1) return 1;
            pAttire[playerid][Slot][slot] = i;
            dySetPlayerAttachedObject(playerid, slot, i);
        }
    }
    return 1;
}
stock dySetPlayerAttachedObject(const playerid, const slot, const id) //�������װ��
{
    SetPlayerAttachedObject(playerid, slot, AttireInfo[playerid][id][ModelID], AttireInfo[playerid][id][Bone], AttireInfo[playerid][id][OffsetX],
        AttireInfo[playerid][id][OffsetY], AttireInfo[playerid][id][OffsetZ], AttireInfo[playerid][id][RotX], AttireInfo[playerid][id][RotY], AttireInfo[playerid][id][RotZ],
        AttireInfo[playerid][id][ScaleX], AttireInfo[playerid][id][ScaleY], AttireInfo[playerid][id][ScaleZ], AttireInfo[playerid][id][Color1], AttireInfo[playerid][id][Color2]);
}
stock GetSlot(const playerid) //��ȡδʹ�õĲ�id
{
    for (new i = 0; i < MAX_PLAYER_ATTACHED_OBJECTS - 1; i++) {
        if(pAttire[playerid][Slot][i] == -1) {
            return i;
        }
    }
    return -1;
}
stock GetEditSlotID(const playerid) //��ȡ�༭��id
{
    for (new i = 0; i < MAX_PLAYER_ATTACHED_OBJECTS; i++) {
        if(pAttire[playerid][Slot][i] == pAttire[playerid][SelectID]) {
            return i;
        }
    }
    return -1;
}
stock GetPlayerAttireCount(const playerid) //��ȡ���װ������
{
    new count;
    for (new i = 0; i < MAX_PLAYER_ATTIRE; i++) {
        if(AttireInfo[playerid][i][DBID] != 0) {
            count++;
        }
    }
    return count;
}
stock ChangeAttireState(const playerid, const id, const cstate) //����װ��״̬
{
    new Query[128];
    AttireInfo[playerid][id][State] = cstate;
    mysql_format(g_Sql, Query, sizeof Query, "UPDATE `attire` SET `State` = '%d' WHERE `DBID` = '%d'", \
    AttireInfo[playerid][id][State], AttireInfo[playerid][id][DBID]);
    mysql_pquery(g_Sql, Query);
}
stock UpdateAttireData(const playerid, const id) //����װ������
{
    new Query[256];
    mysql_format(g_Sql, Query, sizeof(Query), "UPDATE `attire` SET `Name` = '%e', `ModelID` = '%d', `Bone` = '%d', `OffsetX` = '%f', `OffsetY` = '%f', `OffsetZ` = '%f', \
    `RotX` = '%f', `RotY` = '%f', `RotZ` = '%f' WHERE `DBID` = '%d'", AttireInfo[playerid][id][Name], AttireInfo[playerid][id][ModelID], \
    AttireInfo[playerid][id][Bone], AttireInfo[playerid][id][OffsetX], AttireInfo[playerid][id][OffsetY], AttireInfo[playerid][id][OffsetZ], \
    AttireInfo[playerid][id][RotX], AttireInfo[playerid][id][RotY], AttireInfo[playerid][id][RotZ], AttireInfo[playerid][id][DBID]);
    mysql_pquery(g_Sql, Query);

    mysql_format(g_Sql, Query, sizeof(Query), "UPDATE `attire` SET `ScaleX` = '%f', `ScaleY` = '%f', `ScaleZ` = '%f', `Color1` = '%d', `Color2` = '%d', `State` = '%d' \
    WHERE DBID = '%d'", AttireInfo[playerid][id][ScaleX], AttireInfo[playerid][id][ScaleY], AttireInfo[playerid][id][ScaleZ], \
    AttireInfo[playerid][id][Color1], AttireInfo[playerid][id][Color2], AttireInfo[playerid][id][State], AttireInfo[playerid][id][DBID]);
    mysql_pquery(g_Sql, Query);
}
stock Attire_EditAttachedObject(const playerid, const response, const index, /*modelid, boneid,*/ Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ) {
    if(response == 1) {
        new id = pAttire[playerid][SelectID];
        AttireInfo[playerid][id][OffsetX] = fOffsetX, AttireInfo[playerid][id][OffsetY] = fOffsetY, AttireInfo[playerid][id][OffsetZ] = fOffsetZ;
        AttireInfo[playerid][id][RotX] = fRotX, AttireInfo[playerid][id][RotY] = fRotY, AttireInfo[playerid][id][RotZ] = fRotZ;
        AttireInfo[playerid][id][ScaleX] = fScaleX, AttireInfo[playerid][id][ScaleY] = fScaleY, AttireInfo[playerid][id][ScaleZ] = fScaleZ;
        RemovePlayerAttachedObject(playerid, index);
        dySetPlayerAttachedObject(playerid, index, id);
        UpdateAttireData(playerid, id);
    } else {
        dySetPlayerAttachedObject(playerid, index, pAttire[playerid][SelectID]);
    }
    return 0;
}
stock ShowPlayerAttireDialog(const playerid) //���װ��Ի���
{
    new msg[2048], title[128];
    format(title, sizeof title, "�ҵ�װ��(%d/%d)", GetPlayerAttireCount(playerid), MAX_PLAYER_ATTIRE);
    format(msg, sizeof msg, "װ����\t��λ\t״̬");
    for (new i = 0; i < MAX_PLAYER_ATTIRE; i++) {
        if(AttireInfo[playerid][i][DBID] == 0) {
            format(msg, sizeof msg, "%s\n \t \t ", msg);
        } else {
            format(msg, sizeof msg, "%s\n%s\t%s\t%s", msg, AttireInfo[playerid][i][Name], AttireBoneNames[AttireInfo[playerid][i][Bone]], AttireStateNames[AttireInfo[playerid][i][State]]);
        }
    }
    Dialog_Show(playerid, Attire, DIALOG_STYLE_TABLIST_HEADERS, title, msg, "ѡ��", "����");
}
stock ShowPlayerAttireSelect(const playerid) //���װ������֮һ�ĶԻ���(����pAttire[playerid][SelectID]ʹ��)
{
    new msg[256], id = pAttire[playerid][SelectID];
    format(msg, sizeof msg, "____________________|װ����Ϣ|____________________\n");
    format(msg, sizeof msg, "%sװ������:%s\nװ�粿λ:%s\nװ��DBID:%d\nװ��״̬:%s\n", msg, AttireInfo[playerid][id][Name], AttireBoneNames[AttireInfo[playerid][id][Bone]],
        AttireInfo[playerid][id][DBID], AttireStateNames[AttireInfo[playerid][id][State]]);
    format(msg, sizeof msg, "%s____________________|�����༭|____________________\n", msg);
    format(msg, sizeof msg, "%s�༭λ��\n���Ĳ�λ\n����װ��\n����װ��\n{FF0000}����װ��", msg);
    Dialog_Show(playerid, Attire_Select, DIALOG_STYLE_LIST, "�ҵ�װ��", msg, "ѡ��", "����");
}
Dialog:Attire(playerid, response, listitem, inputtext[]) {
    if(!response) {
        OnPlayerSettings(playerid); //�ҵ�װ�����
        return 1;
    }
    pAttire[playerid][SelectID] = -1;
    if(AttireInfo[playerid][listitem][DBID] == 0) {
        SendClientMessage(playerid, Color_Red, "[װ��]���в���ѡ��.");
        return 1;
    }
    pAttire[playerid][SelectID] = listitem;
    ShowPlayerAttireSelect(playerid);
    return 1;
}
Dialog:Attire_Select(playerid, response, listitem, inputtext[]) {
    if(!response) {
        ShowPlayerAttireDialog(playerid);
        return 1;
    }
    new slot;
    if(mk_strcmp(inputtext, "�༭λ��") == 0) {
        slot = GetEditSlotID(playerid);
        if(slot == -1) {
            SendClientMessage(playerid, Color_Red, "[װ��]��װ��δװ��...(�﷨����)");
            return 1;
        }
        EditAttachedObject(playerid, slot);
        return 1;
    }
    if(mk_strcmp(inputtext, "���Ĳ�λ") == 0) {
        new msg[128];
        for (new i = 1; i < sizeof(AttireBoneNames); i++) {
            format(msg, sizeof msg, "%s\n%s", msg, AttireBoneNames[i]);
        }
        Dialog_Show(playerid, Attire_Bone, DIALOG_STYLE_LIST, "�ҵ�װ�� - ��λ����", msg, "����", "����");
        return 1;
    }
    if(mk_strcmp(inputtext, "����װ��") == 0) {
        if(AttireInfo[playerid][pAttire[playerid][SelectID]][State] == 0) {
            SendClientMessage(playerid, Color_Red, "[װ��]��δ����, ��������.");
            return 1;
        }
        slot = GetEditSlotID(playerid);
        if(slot == -1) {
            SendClientMessage(playerid, Color_Red, "[װ��]δ֪����, ����ϵ����Ա.(take off return value -1)");
            return 1;
        }
        pAttire[playerid][Slot][slot] = -1;
        ChangeAttireState(playerid, pAttire[playerid][SelectID], 0);
        RemovePlayerAttachedObject(playerid, slot);
        SendClientMessage(playerid, Color_White, "[װ��]�����������װ��.");
        return 1;
    }
    if(mk_strcmp(inputtext, "����װ��") == 0) {
        if(AttireInfo[playerid][pAttire[playerid][SelectID]][State] == 1) {
            SendClientMessage(playerid, Color_Red, "[װ��]�Ѿ�������.(�㵽��Ҫ�����ٴ�, ����)");
            return 1;
        }
        slot = GetSlot(playerid);
        if(slot == -1) {
            SendClientMessage(playerid, Color_Red, "[װ��]�㲻�ܴ�������..");
            return 1;
        }
        pAttire[playerid][Slot][slot] = pAttire[playerid][SelectID];
        ChangeAttireState(playerid, pAttire[playerid][SelectID], 1);
        dySetPlayerAttachedObject(playerid, slot, pAttire[playerid][SelectID]);
        SendClientMessage(playerid, Color_White, "[װ��]�㴩�������װ��.");
        return 1;
    }
    if(mk_strcmp(inputtext, "����װ��") == 0) {
        new Float:x, Float:y, Float:z, msg[128];
        GetPlayerPos(playerid, x, y, z);
        format(msg, sizeof msg, "[װ��]�㶪������� %s.", AttireInfo[playerid][pAttire[playerid][SelectID]][Name]);
        SendClientMessage(playerid, Color_White, msg);
        Attire_Delete(AttireInfo[playerid][pAttire[playerid][SelectID]][DBID]);
        // Furniture_Create("discarded_attire", AttireInfo[playerid][pAttire[playerid][SelectID]][ModelID], x, y, z - 0.5, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));
        Attire_ReadData(playerid);
        SpawnAttire(playerid);
        SetPlayerPos(playerid, x, y, z + 1);
        return 1;
    }
    return 1;
}
Dialog:Attire_Bone(playerid, response, listitem, inputtext[]) {
    if(!response) {
        ShowPlayerAttireSelect(playerid);
        return 1;
    }
    new slot = GetEditSlotID(playerid);
    if(slot == -1) {
        SendClientMessage(playerid, Color_Red, "[װ��]���ȴ��ϸ�װ����ʹ�øù��ܽ��и�����λ.");
        return 1;
    }
    new msg[128];
    AttireInfo[playerid][pAttire[playerid][SelectID]][Bone] = listitem + 1;
    mysql_format(g_Sql, msg, sizeof msg, "UPDATE `attire` SET `Bone` = '%d' WHERE `DBID` = '%d'", AttireInfo[playerid][pAttire[playerid][SelectID]][Bone], \
    AttireInfo[playerid][pAttire[playerid][SelectID]][DBID]);
    mysql_pquery(g_Sql, msg);
    RemovePlayerAttachedObject(playerid, slot);
    dySetPlayerAttachedObject(playerid, slot, pAttire[playerid][SelectID]);
    format(msg, sizeof msg, "[װ��]��λ���ĳɹ�.[%s]", AttireBoneNames[AttireInfo[playerid][pAttire[playerid][SelectID]][Bone]]);
    SendClientMessage(playerid, Color_White, msg);
    return 1;
}
Dialog:Attire_SellList(playerid, response, listitem, inputtext[]) {
    if(!response) return 1;
    new msg[128], idx;
    msg = strtok(inputtext, idx);
    if(mk_strcmp(msg, "��һҳ") == 0) {
        if(pAttire[playerid][Page] == 1) {
            SendClientMessage(playerid, Color_Red, "[װ��]û����һҳ��, ���ǵ�һҳ.");
            return 1;
        }
        pAttire[playerid][Page]--;
        ShowPlayerAttireSellList(playerid, pAttire[playerid][Page]);
        return 1;
    }
    if(mk_strcmp(msg, "��һҳ") == 0) {
        new maxpage; // = GetMaxPage(AttireSellCount);
        if(AttireSellCount % 10 == 0) maxpage = AttireSellCount / 10; //һҳ10��
        else maxpage = (AttireSellCount / 10) + 1;
        if(pAttire[playerid][Page] >= maxpage) {
            SendClientMessage(playerid, Color_Red, "[װ��]û����һҳ��, �������һҳ.");
            return 1;
        }
        pAttire[playerid][Page]++;
        ShowPlayerAttireSellList(playerid, pAttire[playerid][Page]);
        return 1;
    }
    pAttire[playerid][PreviewID] = strval(msg);
    for (new i = 0; i < MAX_PLAYER_ATTACHED_OBJECTS; i++) {
        if(IsPlayerAttachedObjectSlotUsed(playerid, i) == 1) {
            RemovePlayerAttachedObject(playerid, i);
        }
    }
    SendClientMessage(playerid, Color_White, "[װ��]Ԥ��������һ��'4'��һ��'6'");
    Attire_Preview(playerid, pAttire[playerid][PreviewID]);
    return 1;
}
Dialog:Attire_Buyit(playerid, response, listitem, inputtext[]) {
    if(!response) {
        return 1;
    }
    new id = pAttire[playerid][PreviewID];
    pAttire[playerid][PreviewID] = -1;
    SpawnAttire(playerid);
    if(PlayerInfo[playerid][Cash] < AttireSell[id][Price]) {
        SendClientMessage(playerid, Color_Red, "[װ��]���Ǯ����.");
        return 1;
    }
    if(GetPlayerAttireCount(playerid) >= MAX_PLAYER_ATTIRE) {
        SendClientMessage(playerid, Color_Red, "[װ��]�㲻���Թ�������װ����..");
        return 1;
    }
    new msg[128];
    GivePlayerCash(playerid, -AttireSell[id][Price]);
    // GivePlayeriXyxb(playerid, -AttireSell[id][Price]);
    CreateAttire(PlayerInfo[playerid][ID], AttireSell[id][Name], AttireSell[id][OBJID], AttireSell[id][preview_bone], AttireSell[id][preview_offsetx], AttireSell[id][preview_offsety],
        AttireSell[id][preview_offsetz], AttireSell[id][preview_rotx], AttireSell[id][preview_roty], AttireSell[id][preview_rotz],
        AttireSell[id][preview_scalex], AttireSell[id][preview_scaley], AttireSell[id][preview_scalez]);
    format(msg, sizeof msg, "[װ��]%s ����ɹ�, ������ $%d, �ѷ������װ����.", AttireSell[id][Name], AttireSell[id][Price]);
    SendClientMessage(playerid, Color_White, msg);
    Attire_ReadData(playerid);
    return 1;
}


public OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ) {
    if(Attire_EditAttachedObject(playerid, response, index, /*modelid, boneid,*/ fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ) == 1) return 1;
    return 1;
}
CMD:wdzb(playerid, cmdtext[]) {
    ShowPlayerAttireDialog(playerid);
    return 1;
}
// ��ʼ����
stock SetupAttireTable() {
    mysql_pquery(g_Sql,"CREATE TABLE IF NOT EXISTS `attire`  (\
    `DBID` int(11) UNSIGNED NOT NULL AUTO_INCREMENT,\
    `UID` int(10) UNSIGNED NULL DEFAULT NULL,\
    `Name` varchar(24) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `ModelID` int(11) NULL DEFAULT NULL,\
    `Bone` int(11) NULL DEFAULT NULL,\
    `OffsetX` varchar(11) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `OffsetY` varchar(11) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `OffsetZ` varchar(11) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `RotX` varchar(11) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `RotY` varchar(11) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `RotZ` varchar(11) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `ScaleX` varchar(11) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `ScaleY` varchar(11) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `ScaleZ` varchar(11) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,\
    `Color1` int(11) NULL DEFAULT NULL,\
    `Color2` int(11) NULL DEFAULT NULL,\
    `State` int(11) NULL DEFAULT NULL,\
    PRIMARY KEY (`DBID`) USING BTREE\
    ) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;");
}