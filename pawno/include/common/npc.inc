// IORP Script
/*
    This server is modified from 5F free residential open source, Prace open source
    To the makers of Prace:[Ghost]Rui ushio_p [Ghost]Dylan
    Tribute to the makers of the free settlement :[ITC]dyq [ITC]fangye [ITC]Super_wlc [ITC
    Special thanks to ryddawn and technical advisor [Fire]KiVen OBJ:JoshenKM;

								The MIT License  
								
	Copyright (c) <2019-2020> <YuCarl77>  
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.  
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.  
	==================================
	��Ȩ(c) <2019-2020> <YuCarl77>   

	ʹ�ø�����֤����������������Ȩ�ޣ���ѣ��κ��˿��Եõ����������������ĵ���һ��������
	���Ҿ�Ӫ�����������κ����ƣ����������Ƶ�ʹ�á����ơ��޸ġ��ϲ������桢���С����Ŵ���֤�顢���߳��۸������Ŀ�����Ȩ����
	ͬʱ���������Щ�������û�������ЩȨ����ʹ����������������  
	
	���ϵİ�Ȩ֪ͨ��Ȩ��֪ͨӦ�ð��������и������Ŀ����л�������������������ʵ�����С�
	
	�������������������ṩ��û���κ���ʽ�ĵ�������������ȷ�ػ��߰����ģ�������Щ���ǲ�����ҵ���ʵĵ��������ơ�
	�ʺ�һ���ض�����;���Ҳ����ַ������ߺͰ�Ȩ���������κγ��϶�ʹ�ø������漰���κ�Ҫ���𺦻����������ζ���Ӧ����
	�����������������û���ֻ�Ǻ�ͬ��ʽ��������Ȩ����������ʽ�������������������÷�Χ�ڡ������������ϵ����������ʹ�û�����������������������Ϊ��  
	=====================================   
*/

//  �����ң����ڿ�Դ��д��лǰ�����˵����������֮���١�
//  �����޸���5F��Prace,PHouse,Goods�ȿ�Դ �ر���л��[Ghost]Rui ushio_p [Ghost]Dylan
//  �¾����ɾ�������Դ,������:[ITC]dyq  [ITC]fangye  [ITC]Super_wlc [ITC]RR_LXD  mk124  Shindo(aka. ssh)  vvg, yezizhu(aka. yzz)
//  �ر���л ryddawn ������OBJָ����[Fire]KiVen JoshenKM
//  �ر���л GoodsSys Episodes KiVen juse
//  �ر���л GTAUN GTABBS 
// RST�Ŷӷ�����������Ҫ��[R_ST]Hygen��YuCarl77���쵼

#define VERSION "RST�Ŷ�NPCϵͳ�޸İ�" // Դ Shindo ��ȡ��δ������NPC
#define TOTAL_NPC 8

//DEBUG Settings
// #define DEBUG

//Defines

// #define SHOW_BOUNDS "/showbounds"
// #define NPC_DIALOG_ID 102

//Colors
#define RED 0xA10000AA
#define GREEN 0x33AA33AA

//Vars
new bool:NPCNameShow;
// new bool:BoundsShow;
new Text3D:textid[TOTAL_NPC];
// new NPCIDs[TOTAL_NPC];

//Vehicles

new npcCars[8];

stock LoadNpcs() {
    npcCars[0] = AddStaticVehicleEx(541, -342.7927, 1540.4495, 75.1911, 178.9945, 6, 1, -1);
    //Elegy1 = AddStaticVehicle(562,0.0,0.0,5.0,0.0,6,1); - �ǰɵ�
    npcCars[1] = AddStaticVehicleEx(562, 1.0, 1.0, 5.0, 0.0, 7, 1, -1);
    npcCars[2] = AddStaticVehicleEx(411, 3.0, 3.0, 5.0, 0.0, 6, 1, -1);
    npcCars[3] = AddStaticVehicleEx(411, 5.0, 5.0, 5.0, 0.0, 118, 1, -1);
    npcCars[4] = AddStaticVehicleEx(411, 7.0, 7.0, 5.0, 0.0, 66, 1, -1);
    npcCars[5] = AddStaticVehicleEx(411, 9.0, 9.0, 5.0, 0.0, 65, 1, -1);
    npcCars[6] = AddStaticVehicleEx(411, 15.0, 9.0, 5.0, 0.0, 68, 1, -1);
    npcCars[7] = AddStaticVehicleEx(411, 18.0, 9.0, 5.0, 0.0, 70, 1, -1);
    // for(new i = 0; i < sizeof(npcCars); i++) {
    //     AddVehicleComponent(npcCars[i], 1010); //��NPCһ������
    // }
    secondload_npcs();
    // BoundsShow = true;
    printf("\n* Shindo's NPC System - LXD�ƻ� %s �Ѽ���.\n", VERSION);
}
stock secondload_npcs(){
    ConnectNPC("DrifterLDZ", "LXDLDZ");
    // SetTimer("secondload_npcstwo", 100, false);
    SetTimer_("secondload_npcstwo", 100, 100, 1);
    //
    //ConnectNPC("Drifter1","Drift1"); - �ǰɵ�
    return 1;
}
function secondload_npcstwo() {
    ConnectNPC("Drifter", "Drift");
    // SetTimer("secondload_npcsthree", 100, false);
    SetTimer_("secondload_npcsthree", 100, 100, 1);
}
function secondload_npcsthree() {
    ConnectNPC("DrifterMadd", "RunInMadd");
    // SetTimer("secondload_npcsfour", 100, false);
    SetTimer_("secondload_npcsfour", 100, 100, 1);
}
function secondload_npcsfour() {
    ConnectNPC("DrifterLS", "LSEyeOut");
    // SetTimer("secondload_npcsfive", 100, false);
    SetTimer_("secondload_npcsfive", 100, 100, 1);
}
function secondload_npcsfive() {
    ConnectNPC("DrifterTD", "TestDrive");
    // SetTimer("secondload_npcssix", 100, false);
    SetTimer_("secondload_npcssix", 100, 100, 1);
}
function secondload_npcssix() {
    ConnectNPC("DrifterOff", "OffControl");
    // SetTimer("secondload_npcsseven", 100, false);
    SetTimer_("secondload_npcsseven", 100, 100, 1);

}
function secondload_npcsseven() {
    ConnectNPC("DrifterFM", "FollowMe");
    // SetTimer("secondload_npcseight", 100, false);
    SetTimer_("secondload_npcseight", 100, 100, 1);

}
function secondload_npcseight() {
    ConnectNPC("DrifterFM2", "FollowMe2");
}
stock UnLoadNpcs() {
    for (new i = 0; i < GetMaxPlayers(); i++) {
        if(IsPlayerNPC(i)) Kick(i);
    }
    // printf("\n* Shindo's NPC System - LXD�ƻ� %s ���˳�.\n", VERSION);
}
stock InitializationNpcs(const playerid) {
    // SetSpawnInfo(playerid, 0, 115, 1958.33, 1343.12, 15.36, 269.15, 0, 0, 0, 0, 0, 0);
    new string[50];
    if(!strcmp(GetName(playerid), "DrifterLDZ", true)) {
        PutPlayerInVehicle(playerid, npcCars[0], 0);
        format(string, sizeof(string), "Drifter-LDZ\n(ID:%d)", playerid);
        textid[0] = CreateDynamic3DTextLabel(string, GREEN, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[0]);
        return 1;
    }
    if(!strcmp(GetName(playerid), "Drifter", true)) {
        PutPlayerInVehicle(playerid, npcCars[1], 0);
        format(string, sizeof(string), "Drifter-SF Two Circles\n(ID:%d)", playerid);
        textid[1] = CreateDynamic3DTextLabel(string, GREEN, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[1]);
        return 1;
    }
    if(!strcmp(GetName(playerid), "DrifterMadd", true)) {
        PutPlayerInVehicle(playerid, npcCars[2], 0);
        format(string, sizeof(string), "Drifter-RunInMadd\n(ID:%d)", playerid);
        // NPCIDs[2] = playerid;
        textid[2] = CreateDynamic3DTextLabel(string, GREEN, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[2]);
        return 1;
    }
    if(!strcmp(GetName(playerid), "DrifterLS", true)) {
        PutPlayerInVehicle(playerid, npcCars[3], 0);
        format(string, sizeof(string), "Drifter-LSEyeOut\n(ID:%d)", playerid);
        // NPCIDs[3] = playerid;
        textid[3] = CreateDynamic3DTextLabel(string, GREEN, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[3]);
        return 1;
    }
    if(!strcmp(GetName(playerid), "DrifterTD", true)) {
        PutPlayerInVehicle(playerid, npcCars[4], 0);
        format(string, sizeof(string), "Drifter-SFTestDrive\n(ID:%d)", playerid);
        // NPCIDs[4] = playerid;
        textid[4] = CreateDynamic3DTextLabel(string, GREEN, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[4]);
        return 1;
    }
    if(!strcmp(GetName(playerid), "DrifterOff", true)) {
        PutPlayerInVehicle(playerid, npcCars[5], 0);
        format(string, sizeof(string), "Drifter-OffControl(Rally)\n(ID:%d)", playerid);
        // NPCIDs[5] = playerid;
        textid[5] = CreateDynamic3DTextLabel(string, GREEN, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[5]);
        return 1;
    }
    if(!strcmp(GetName(playerid), "DrifterFM", true)) {
        PutPlayerInVehicle(playerid, npcCars[6], 0);
        format(string, sizeof(string), "Drifter-FollowMe\n(ID:%d)", playerid);
        // NPCIDs[6] = playerid;
        textid[6] = CreateDynamic3DTextLabel(string, GREEN, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[6]);
        return 1;
    }
    if(!strcmp(GetName(playerid), "DrifterFM2", true)) {
        PutPlayerInVehicle(playerid, npcCars[7], 0);
        format(string, sizeof(string), "Drifter-FollowMe2\n(ID:%d)", playerid);
        // NPCIDs[7] = playerid;
        textid[7] = CreateDynamic3DTextLabel(string, GREEN, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, npcCars[7]);
        return 1;
    }
    return 1;
}
function SetPlayerToNPCVehicle(playerid, vehicleid) {
    if(IsPlayerInVehicle(playerid, vehicleid)) return SendClientMessage(playerid, RED, "[����:] ���Ѿ�����Ӧ��NPC�ĳ�����.");
    new result;
    enum piEn {
        piEnName[MAX_PLAYER_NAME],
		piEnid
    };
    new pi[piEn];
    for (new i = 0; i < MAX_PLAYERS; i++) {
        if(IsPlayerConnected(i) && !IsPlayerNPC(i)) {
            if(i != playerid) {
                if(IsPlayerInVehicle(i, vehicleid)) {
                    result = 1;
                    pi[piEnid] = i;
                    GetPlayerName(i, pi[piEnName], sizeof(pi[piEnName]));
                    break;
                }{
                    result = 0;
                }
            }
        }
    }
    if(result) {
        new string[128];
        format(string, sizeof(string), "[����:] ָ����NPC�ĳ����Ѿ��г˿���,�ó˿���%s(%d),ʹ��/tv��ѡ��ó˿͵�IDҲ���Կ�NPC.", pi[piEnName], pi[piEnid]);
        return SendClientMessage(playerid, RED, string);
    }
    PutPlayerInVehicle(playerid, vehicleid, 1);
    SendClientMessage(playerid, GREEN, "[��ʾ:] ���ͳɹ�, ��F�³������뿪NPC����.");
    return 1;
}
Dialog:NPC_DIALOG_ID(playerid, response, listitem, inputtext[]) {
    if(response) {
        switch (listitem) {
            case 0:{            
                SetPlayerToNPCVehicle(playerid, npcCars[0]);
            }
            case 1:{
                SetPlayerToNPCVehicle(playerid, npcCars[4]);
            }
            case 2:{
                SetPlayerToNPCVehicle(playerid, npcCars[2]);
            }
            case 3:{
                SetPlayerToNPCVehicle(playerid, npcCars[3]);
            }
            case 4:{
                SetPlayerToNPCVehicle(playerid, npcCars[5]);
            }
            case 5:{
                SetPlayerToNPCVehicle(playerid, npcCars[1]);
            }
            case 6:{
                SetPlayerToNPCVehicle(playerid, npcCars[6]);
            }
            case 7:{
                SetPlayerToNPCVehicle(playerid, npcCars[7]);
            }
        }
        return 1;
    }
    return 1;
}
CMD:selectnpc(const playerid, const cmdtext[]) {
    new content[1500];
    format(content, sizeof(content), "1.�״�վNPC\n\
                                    2.SF����NPC\n\
                                    3.LSɽ��NPC\n\
                                    4.LS����NPC\n\
                                    5.���н���ԽҰNPC\n\
                                    6.SF˫��NPC\n\
                                    7.�ۺ�NPC - FollowMe\n\
                                    8.�ۺ�NPC - FollowMe2");
    Dialog_Show(playerid, NPC_DIALOG_ID, DIALOG_STYLE_LIST, "��ѡ��һ��NPC, Ȼ���㽫�ᱻ���͵���Ӧ��NPC�ĳ���.", content, "ȷ��", "ȡ��");
    return 1;
}