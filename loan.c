#include <stdio.h>
#include <stdlib.h>

/* 等额本息
 * 每个月还款的总额是固定值
 * P(n) - 第n个月时还的本金
 * S(n) - 第n个月还款前剩余的本金
 * r	- 月利率
 * 则有：
 * S(0)*r+P(0) = S(1)*r+P(1)
 * S(1) = S(0)-P(0)
 * =>S(0)*r+P(0) = S(0)*r-P(0)*r+P(1)
 * =>P(1) = P(0)*(1+r)
 * =>P(n) = P(0)*(1+r)^n
 * @param	sum			总额
 * @param	month		月数
 * @param	rate_y		年利率
 */
void loan_debx(unsigned int sum, unsigned int month, double rate_y)
{
	unsigned int cnt;
	double		 rate_m = rate_y/12;	// 月利率
	double		 f_p, f_p_x, f_p_sum;
	double		 f_i_x, f_i_sum;
	double		 f_a_x, f_a_sum;
	double		 f_b_x;
	double		 f_tmp;
	FILE*		 fp;
	char		 buff[128];
	
	fp = fopen("debx.txt", "wb+");
	if(NULL == fp)
	{
		printf("open file <debx.txt> fail\r\n");
		return;
	}
	
	sprintf(buff, "等额本息借款[%d 元] [%d 月] [利率%.02f%%]\r\n", sum, month, rate_y*100);
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	//				%03d : %5.02f ->[%5.02f] [%5.02f]\r\n
	sprintf(buff, "分期   月供      本金      利息\r\n");
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	// 算出所有月的还款本金对第一个月还款本金的权的和
	f_tmp = 0;
	for(cnt=0; cnt<month; cnt++)
	{
		f_tmp += pow(1+rate_m, cnt);
	}
	// 算出第一个还款本金(f_p)
	f_p = sum/f_tmp;
	
	f_b_x = sum;
	f_p_sum = 0;
	f_i_sum = 0;
	f_a_sum = 0;
	for(cnt=0; cnt<month; cnt++)
	{
		// 当前月份还款本金(f_p_x)
		f_p_x = f_p*pow(1+rate_m, cnt);
		// 当前月份还款利息(f_i_x)
		f_i_x = f_b_x*rate_m;
		// 当前月份还款总额(f_a_x)
		f_a_x = f_p_x+f_i_x;
		
		sprintf(buff, "%03d : %5.02f ->[%5.02f] [%5.02f]\r\n", cnt+1, f_a_x, f_p_x, f_i_x);
		fwrite(buff, 1, strlen(buff), fp);
		printf("%s", buff);
		
		// 已还本金总额
		f_p_sum += f_p_x;
		// 已还利息总额
		f_i_sum += f_i_x;
		// 已还总额
		f_a_sum += f_a_x;
		// 剩余未还本金
		f_b_x -= f_p_x;
	}
	
	sprintf(buff, "全部还款: %.02f, 本金: %.02f, 利息: %.02f, 剩余本金：%.02f\r\n", f_a_sum, f_p_sum, f_i_sum, f_b_x);
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	fclose(fp);
}

/* 等额本金
 * @param	sum			总额
 * @param	month		月数
 * @param	rate_y		年利率
 */
void loan_debj(unsigned int sum, unsigned int month, double rate_y)
{
	unsigned int cnt;
	double		 rate_m = rate_y/12;
	double		 f_p, f_p_x, f_p_sum;
	double		 f_i_x, f_i_sum;
	double		 f_a_x, f_a_sum;
	double		 f_b_x;
	double		 f_tmp;
	FILE*		 fp;
	char		 buff[128];
	
	fp = fopen("debj.txt", "wb+");
	if(NULL == fp)
	{
		printf("open file <debj.txt> fail\r\n");
		return;
	}
	
	sprintf(buff, "等额本金借款[%d 元] [%d 月] [利率%.02f%%]\r\n", sum, month, rate_y*100);
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	sprintf(buff, "分期   月供      本金      利息\r\n");
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	f_p = (double)sum/(month);
	f_b_x = sum;
	f_p_sum = 0;
	f_i_sum = 0;
	f_a_sum = 0;
	for(cnt=0; cnt<month; cnt++)
	{
		f_p_x = f_p;
		f_i_x = f_b_x*rate_m;
		f_a_x = f_p_x+f_i_x;
		
		sprintf(buff, "%03d : %5.02f ->[%5.02f] [%5.02f]\r\n", cnt+1, f_a_x, f_p_x, f_i_x);
		fwrite(buff, 1, strlen(buff), fp);
		printf("%s", buff);
	
		f_p_sum += f_p_x;
		f_i_sum += f_i_x;
		f_a_sum += f_a_x;
		
		f_b_x -= f_p_x;
	}
	
	sprintf(buff, "全部还款: %.02f, 本金: %.02f, 利息: %.02f, 剩余本金：%.02f\r\n", f_a_sum, f_p_sum, f_i_sum, f_b_x);
	fwrite(buff, 1, strlen(buff), fp);
	printf("%s", buff);
	
	fclose(fp);
}

int main(void)
{
	FILE* fp;
	unsigned int sum;
	unsigned int month;
	double        rate;
	char		 buff[128];
	char		 tmp[128];
	int			 tmp_index=0, flag=0;
	int 		 len, cnt;
	
	fp = fopen("cfg.txt", "r");
	if(NULL == fp)
	{
		printf("open <cfg.txt> fail\r\n");

		fp = fopen("..\\cfg.txt", "r");
		if(NULL == fp)
		{
			printf("open <..\\cfg.txt> fail\r\n");
			return 0; 
		}
	}
	
	len = fread(buff, 1, 127, fp);
	buff[len] = 0;
	
	if(len > 0)
	{
		for(cnt=0; cnt<len+1; cnt++)
		{
			if('\r'==buff[cnt] || '\n'==buff[cnt] || '\0' == buff[cnt])
			{
				// "\r\n"的情况处理
				if(0==tmp_index)
					continue;

				tmp[tmp_index] = 0;
				switch(flag)
				{
					// 总额
					case 0:
						sum = atoi(tmp);
						flag = 1;
						break;
					// 月数	
					case 1:
						month = atoi(tmp);
						flag = 2;
						break;
					// 年利率	
					case 2:
						rate = atof(tmp);
						flag = 3;
						break;
					// 多余的数据	
					case 3:
						flag = 3;
						break;
					default:
						flag = 0;
						break;
				}
				tmp_index = 0;
				if('\0' == buff[cnt])
				{
					break;
				}
			}
			else
			{
				tmp[tmp_index++] = buff[cnt];
			}
		}
		if(3 == flag)
		{
			printf("sum: %d, month: %d, rate: %f\r\n", sum, month, rate);
			if(0==month || 0==sum || 0==rate)
				return 0;
		}
		else
		{
			printf("Parse cfg.txt error(%u)\r\n", flag);
			return 0;
		}
	}
	fclose(fp);
	
	loan_debx(sum, month, rate);
	loan_debj(sum, month, rate);
}

