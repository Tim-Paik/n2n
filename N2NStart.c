/*
�Զ���ipʲô�����޸����¼��У����ע�ͣ�������ٶ�����#defineʹ��
*/
#define IPADRESS "dhcp:192.168.211.100"//�Զ��屾��IP,DHCP����DHCP������IPǰ�ӣ�"dhcp:"
#define DHCP "-r"//���ΪDHCP�뽫�����Ϊ��"-r",����DHCP�����գ�""��
#define GROUPNAME "dhwpcs"//��д����
#define PASSWORD "dhwpcs"//��д����
#define SUPERNODEIP "103.205.253.66"//����ΪSuperNode������IP(����)
#define SUPERNODEPORT "10120"//����ΪSuperNode�������˿�
#define OTHERARG " -r -A"//��������
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	char str[]="SOFTWARE\\call";
	HKEY ck;
	char buffer[2048];
	char temp[2048];
	char edgedir[2048];
	char tapdir[2048];
	char vbedir[2048];
	char edgearg[2048];
	char startdir[2048];
	char regdir[2048];
	_getcwd(buffer,2048);//��ȡ��ǰĿ¼
	sprintf(temp,"%s%s",buffer,"\\");//����Ŀ¼
	sprintf(edgedir,"%s%s",temp,"edge.exe");//����edge.exeĿ¼
	sprintf(tapdir,"%s%s",temp,"tap-windows.exe");//����tap-windows.exe��װ��Ŀ¼
	sprintf(vbedir,"%s%s",temp,"start.vbe");//����������vbeĿ¼	
	sprintf(edgearg,"%s%s%s%s%s%s%s%s%s%s%s%s",DHCP," -a ",IPADRESS," -c ",GROUPNAME," -k ",PASSWORD," -l ",SUPERNODEIP,":",SUPERNODEPORT,OTHERARG);//���徲Ĭ����vbe����
	sprintf(startdir,"%s%s%s%s%s%s","CreateObject(\"WScript.Shell\").Run \"cmd /c ","\"\"",edgedir,"\"\" ",edgearg,"\",0");//���徲Ĭ����vbe����
	sprintf(regdir,"%s%s%s","\"",vbedir,"\"");//����������vbeĿ¼
start:
	//������ͨ��ע����ж�tap-windows�Ƿ�װ
	if (ERROR_SUCCESS==RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\TAP-Windows",0,KEY_ALL_ACCESS,&ck)) {
		if (access(edgedir,0)) {
			//MessageBox(NULL,TEXT(edgedir),TEXT("Test!"),0);//edge.exe��Ŀ¼���
			MessageBox(NULL,TEXT("û���ҵ�edge.exe!��������ѹ��edge.exe�뱾����ŵ�ͬһĿ¼������!"),TEXT("Error!"),0);
			if (MessageBox(NULL,TEXT("�Ƿ�ǰ�����أ�"),TEXT("��ʾ"),1)==1){
				system("start https://gitee.com/timpaik/n2n/raw/master/res/edge.exe");
			} else {
				MessageBox(NULL,TEXT("���ֶ����أ�"),TEXT("��ʾ"),0);
				exit(1);
			}
		} else {
			ShellExecute(0,"open","edge.exe",edgearg,"",0);//����n2n
			MessageBox(NULL,TEXT("�����ɹ��������Ҫ�رյĻ����Ե�����������ĺ�̨�ҵ���������edge.exe�������񼴿�"),TEXT("�����ѽ�ͨ"),0);
			if (MessageBox(NULL,TEXT("�Ƿ���ӿ��������"),TEXT("n2n��������"),1)==1){
				FILE *fp;
				fp=fopen(vbedir,"wb");//��ͬĿ¼����start.vbe
				fputs(startdir,fp);//��start.vbe��д����������
				fclose(fp);//�ر��ļ�
				HKEY hKey;//����ע�����
				int length = strlen(regdir);//��ֹ�ں���д��ע�����ʱ���ȹ���
				RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);//��ע���
				RegSetValueEx(hKey,"n2nstart",0,REG_EXPAND_SZ,(unsigned char*)regdir,length);//д��ע�����
				RegCloseKey(hKey);//�ر�ע���
			}
			exit(0);
		}
	} else if (5==RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\TAP-Windows",0,KEY_ALL_ACCESS,&ck)) {//����Ա���
		MessageBox(NULL,TEXT("�����Ƿ��Թ���Ա������У�"),TEXT("����"),0);
		exit(5);
	} else if (2==RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\TAP-Windows",0,KEY_ALL_ACCESS,&ck)) {//tap�������
		MessageBox(NULL,TEXT("δ��װTap���л��������ڴ�Tap��װ������ر�����ɱ�������װTap��"),TEXT("����"),0);
		if (access(tapdir,0)) {//Ѱ��tap��װ��
			MessageBox(NULL,TEXT("û���ҵ�tap-windows.exe!���ֶ���װtap-windows����tap-windows��װ������Ŀ¼��������Ϊtap-windows.exe������!"),TEXT("Error!"),0);
			if (MessageBox(NULL,TEXT("�Ƿ�ǰ�����أ�"),TEXT("��ʾ"),1)==1){
				system("start https://gitee.com/timpaik/n2n/raw/master/res/tap-windows.exe");//��������ҳ
			} else {
				MessageBox(NULL,TEXT("���ֶ����أ�"),TEXT("��ʾ"),0);
			}
			exit(2);
		} else {
			system("tap-windows.exe");//�򿪰�װ��
			goto start;
		}
	}
	return 0;
}
