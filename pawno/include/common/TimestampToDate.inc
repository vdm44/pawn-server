/*
-               Timestamp To Date converter	            -
-                    Made by Jochemd                    -
-          http://forum.sa-mp.com/member.php?u=580      -

native TimestampToDate(Timestamp, &year, &month, &day, &hour, &minute, &second, HourGMT, MinuteGMT = 0);
native DateToTimestamp(str[11]);
*/

/*
               ʱ���ת���� 
ԭ����Jochemd http://forum.sa-mp.com/member.php?u=580  BUG�޸�:YuCarl77([R_ST]Hygen)
�ܱ�Ǹ����λ������Ӱ�죬��BUG���о����������12�²Ż���֣�����ϸ�ܵ��Ų飬�����޸��˸�����©����
��΢BUG:
	ʱ���Ϊ0ʱ������ʾΪ-1 ��1970-1--1 00:00:00
	����ԭ����ԭ���ߵ�д�����У��ɴ���д���ִ���
����BUG(©��):
	�û���¼ʱ����ʾ���һ�ε�¼ʱ�䣬��ʱ���ת��ʱ��if(day > MonthTimes[month][IsLeapYear(year)])�������±�Խ��
����:
	unixʱ���Ϊ��������ʱ��1970��01��01��00ʱ00��00���������ڵ�������
	�������α�׼ʱ�䣨�����������ƽ��ʱ���������α�׼ʱ�䣻Ӣ�GreenwichMeanTime��GMT��
	ָλ��Ӣ���׶ؽ����ĻʼҸ�����������̨�ı�׼ʱ�䣬��Ϊ���������߱�������ͨ������ľ��ߡ�
BUG����ԭ��
	ԭ���ߵ������֮ǰһֱ��������·ݣ�month��ʱ��Ϊ12�¡������±�0~11��Ӧ1-12�£�
	Ҳ����֮ǰһֱ�����1��Ϊ2�£�12��Ϊ13�� �������±굼�¼���ֱ�Ӵ����ޱ���
�Ų���:
	ʱ�����1606780800������ʱ��2020��12��1��8ʱ0��0�뼰�Ժ�ʱ���100%���ִ����⣨HourGMT��
	�й�ʱ����Ӣ��ʱ�����8Сʱ���Ա�׼����ʱ�����8Сʱ��������12��1�� 8ʱ����� 
�޸�ʱ��:
2020��12��26��23ʱ27�� 
*/


#include <a_samp>
#include <sscanf2>

#define SPLITTER .

new MonthTimes[12][4] = 
{
	{ 31, 31, 2678400, 2678400 },
	{ 28, 29, 2419200, 2505600 },
	{ 31, 31, 2678400, 2678400 },
	{ 30, 30, 2592000, 2592000 },
	{ 31, 31, 2678400, 2678400 },
	{ 30, 30, 2592000, 2592000 },
	{ 31, 31, 2678400, 2678400 },
	{ 31, 31, 2678400, 2678400 },
	{ 30, 30, 2592000, 2592000 },
	{ 31, 31, 2678400, 2678400 },
	{ 30, 30, 2592000, 2592000 },
	{ 31, 31, 2678400, 2678400 }
};

// �Ż������ߵĴ���
// stock IsLeapYear(year)
stock IsLeapYear(const year)
{
	// ԭ���߼��㷽����̫�а�
	// �ܱ�400�������ܱ�4���������Ҳ��ܱ�100������������ ������ֻ��4����
	// if(year % 4 == 0) return 1;
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

stock TimestampToDate(Timestamp, &year, &month, &day, &hour, &minute, &second, const HourGMT, const MinuteGMT = 0)
{
	for(year = 1970; ;year++)
	{
		new seconds = IsLeapYear(year) ? 31622400: 31536000;
		if (seconds > Timestamp) break;
		Timestamp -= seconds;
	}		
	for (month = 1; ; month++) {
		new seconds = MonthTimes[month-1][2 + IsLeapYear(year)];
		if (seconds > Timestamp) break;
		Timestamp -= seconds;
	}
	day = 1 + (Timestamp / 86400);
	Timestamp %= 86400;
	hour = HourGMT + (Timestamp / 3600);
	Timestamp %= 3600;
	minute = MinuteGMT + (Timestamp / 60);
	second = (Timestamp % 60);
	if(minute > 59)
	{
		minute = 0;
		hour ++;
	}
	if(hour > 23)
	{
		hour -= 24;
		day ++;
	}	
	if(day > MonthTimes[month-1][IsLeapYear(year)])
	{
		day = 1;
		month ++;
	}
	if(month > 12)
	{
		month = 1;
		year ++;
	}
	return 1;
}


stock DateToTimestamp(str[11])
{
	new date[3]; // date[0] = day		date[1] = month			date[2] = year
	if(!sscanf(str,"p<"#SPLITTER">ddd",date[0],date[1],date[2]))
	{
		new total = 0, tmp = 0;
		total += date[0] * 86400;
		if(date[1] == 2 && date[0] < 29) tmp = ((date[2] - 1968) / 4 - 2);
		else tmp = ((date[2] - 1968) / 4 - 1);
		total += tmp * 31622400;
		total += (date[2] - 1970 - tmp) * 31536000;
		for(new i = 1; i < date[1]; i ++) total += MonthTimes[i][0 + IsLeapYear(date[2])] * 86400;
		return total;
	}
	else return -1;
}


// ԭ���ߵ�BUGд��
// stock TimestampToDateOld(Timestamp, &year, &month, &day, &hour, &minute, &second, HourGMT, MinuteGMT = 0)
// {
// 	new tmp = 2;
// 	year = 1970;
// 	month = 1;
// 	Timestamp -= 172800; // Delete two days from the current timestamp. This is necessary, because the timestamp retrieved using gettime() includes two too many days.
// 	for(;;)
// 	{
// 		if(Timestamp >= 31536000)
// 		{
// 			year ++;
// 			Timestamp -= 31536000;
// 			tmp ++;
// 			if(tmp == 4)
// 			{
// 				if(Timestamp >= 31622400)
// 				{
// 					tmp = 0;
// 					year ++;
// 					Timestamp -= 31622400;
// 				}
// 				else break;
// 			}
// 		}
// 		else break;
// 	}		
// 	for(new i = 0; i < 12; i ++)
// 	{
// 		if(Timestamp >= MonthTimes[i][2 + IsLeapYear(year)])
// 		{
// 			month ++;
// 			Timestamp -= MonthTimes[i][2 + IsLeapYear(year)];
// 		}
// 		else break;
// 	}
// 	day = 1 + (Timestamp / 86400);
// 	Timestamp %= 86400;
// 	hour = HourGMT + (Timestamp / 3600);
// 	Timestamp %= 3600;
// 	minute = MinuteGMT + (Timestamp / 60);
// 	second = (Timestamp % 60);
// 	if(minute > 59)
// 	{
// 		minute = 0;
// 		hour ++;
// 	}
// 	if(hour > 23)
// 	{
// 		hour -= 24;
// 		day ++;
// 	}	
// 	if(day > MonthTimes[month][IsLeapYear(year)])
// 	{
// 		day = 1;
// 		month ++;
// 	}
// 	if(month > 12)
// 	{
// 		month = 1;
// 		year ++;
// 	}
// 	return 1;
// }



// ԭ���ߵ�BUGд����������д�ģ�����ע�͵�
// stock TimestampToDate(Timestamp, &year, &month, &day, &hour, &minute, &second, const HourGMT, const MinuteGMT = 0)
// {
// 	// new tmp = 2;
// 	// 1970�ݶ�tmpΪ2 ��Ϊ1972��tmp����4�� ÿ��4��������
// 	// new tmp = 2;
// 	// ��ʼ���
// 	// year = 1970;
// 	// month = 1;
// 	// Timestamp -= 172800; // Delete two days from the current timestamp. This is necessary, because the timestamp retrieved using gettime() includes two too many days.
// 	// �����������ߵĻ��ӵ�ǰʱ�����ɾ�����졣���Ǳ�Ҫ�ģ���Ϊʹ��gettime()������ʱ����������졣
// 	// ������������������û����Σ�����ͨ��gettime()�ģ�û�ж����죬��Ӧ�ü�ȥ172800
// 	// ������򵥵İ취��˳�����ߵ���˼�Ļ����ж����ֵ�ǲ��Ǵ����������
// 	// Ҳ���Ǿ��Ի��Ľ��������� ���gettime���ǿ��ܳ�������
// 	// if(Timestamp > 172800) Timestamp -= 172800;

// 	// for(;;)
// 	// {
		
// 	// 	// 31536000�Ƿ������һ�������
// 	// 	if(Timestamp >= 31536000)
// 	// 	{
			
// 	// 		year ++;
// 	// 		Timestamp -= 31536000;
// 	// 		tmp ++;
// 	// 		// ÿ��4��������
// 	// 		if(tmp == 4)
// 	// 		{
// 	// 			// 31622400�����������
// 	// 			if(Timestamp >= 31622400)
// 	// 			{
					
// 	// 				// // ÿ��4�������� ������ԭ���ߵ���˼�����������������0�Ļ� �ӵ� 4�ͻ���5���� ���ÿ������������ ����BUG
// 	// 				// tmp = 0;
// 	// 				tmp = 1;
// 	// 				// printf("����:%d",year);
// 	// 				year ++;
// 	// 				Timestamp -= 31622400;
// 	// 			}
// 	// 			else break;
// 	// 		}
// 	// 	}
// 	// 	else break;
// 	// }	

// 	for(year = 1970; ;year++)
// 	{
// 		// 31622400�����������
// 		// 31536000�Ƿ������һ�������
// 		new seconds = IsLeapYear(year) ? 31622400: 31536000;
// 		if (seconds > Timestamp) break;
// 		Timestamp -= seconds;
// 	}		
// 	// ��һ����Ӧ�������·ݣ�����ʱ����Ͷ�Ӧ�Ƿ�Ϊ������
// 	// for(new i = 0; i < 12; i ++)
// 	// {
// 	// 	if(Timestamp >= MonthTimes[i][2 + IsLeapYear(year)])
// 	// 	{
// 	// 		month ++;
// 	// 		Timestamp -= MonthTimes[i][2 + IsLeapYear(year)];
// 	// 	}
// 	// 	else break;
// 	// }
	
// 	for (month = 1; ; month++) {
// 		new seconds = MonthTimes[month-1][2 + IsLeapYear(year)];
// 		if (seconds > Timestamp) break;
// 		Timestamp -= seconds;
// 	}
// 	// ����һ������ʱ���������� ʱ �� ��
// 	// 86400��һ��������


// 	day = 1 + (Timestamp / 86400);
// 	// �ڼ�����ǵ�һ�� + ��һ���ʱ������� / 86400 ��Ϊ��������ǵ�һ��Ļ�/86400����0��
// 	Timestamp %= 86400;
	
// 	// ȡģһ��֮��ʣ�µ������� 
// 	// Сʱ����0~23Сʱ����ʱ������� / 3600  ǰ���GMT����Ϊ������������ʱ�䲻һ�� ���Լ���
// 	hour = HourGMT + (Timestamp / 3600);
// 	// ȡģСʱ֮�������
// 	// 3600��һ��Сʱ������
// 	Timestamp %= 3600;
// 	// �־���0~59�֣���ʱ������� / 60  ǰ���GMT����Ϊ������������ʱ�䲻һ�� ���Լ���
// 	minute = MinuteGMT + (Timestamp / 60);
// 	// �����0~59�룬��ʱ������� ȡ�� 60����ʣ�µľ�������
// 	second = (Timestamp % 60);
// 	// �����>59 ���÷�Ϊ0��Сʱ + 1 ˵����Ҳ��60�������������
// 	if(minute > 59)
// 	{
// 		minute = 0;
// 		hour ++;
// 	}
// 	// ���ʱ>23 ����ʱΪ0���µ�һ�쵽�� ˵����Ҳ��24Сʱ�������������
// 	if(hour > 23)
// 	{
// 		hour -= 24;
// 		day ++;
// 	}	
// 	// if(day > MonthTimes[month][IsLeapYear(year)])
// 	// ԭ���ߺ������Ǽ�1�ˣ������12�µĻ� �����Խ���� ������1~12�� �����±���0~11 û��1
// 	// ���ұ����ʱ���ǲ�����ʾ����ģ���Ϊ�����ֵ�Ǳ�����������Monthֱ�Ӿ���12����±굼�µ��±�Խ��
// 	// ���˰���
// 	// ������ 2020��12��26����
// 	if(day > MonthTimes[month-1][IsLeapYear(year)])
// 	{
// 		day = 1;
// 		month ++;
// 	}
// 	if(month > 12)
// 	{
// 		month = 1;
// 		year ++;
// 	}
// 	return 1;
// }