#include "ext2.h"  /* download this from LAB1 */
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

#define BLK 1024
GD    *gp;
INODE *ip;
DIR   *dp;

u16  i, iblk, inum;
char buf1[BLK], buf2[BLK], boot[6];
char *buf;
u32* ib; 



int prints(char *s)
{
   while(*s!='\0'){
	putc(*s);
	s++;}
	return 0;
}

int gets(char s[]){
	i = 0;
	s[i] = getc();
	while(s[i]!='\r'){
		putc(s[i]);
		i++;
		s[i]=getc();
		}
	putc('0');
	s[i] = '\0';
	
	return 1;
}

u16 getblk(u16 blk, char buf[ ])
{
   //convert blk to (cyl, head, sector);
	//putc(cyl+'0');putc(head+'0');putc(sector+'0');putc(blk+'0');
    readfd(blk/18, (blk/9)%2, (blk*2)%18, buf);
}

u16 ino(char *tmp, char *file){
	buf = tmp;
	dp = (DIR*)buf;
	while(buf < &tmp[1024]){
		if(!strncmp(dp->name,file,dp->name_len)){
			//prints(dp->name);
			return (u16)dp->inode;
			
			}
		buf += dp->rec_len;
		dp = (DIR*)buf;
	}
	return 0;
	}


main()
{ 
 
  prints("booter start\n\r");
	prints("Hopefully this works\n");  
	prints("Enter booter: ");
	//putc('9');
	gets(boot);
	//strcpy(boot,"mtx");
	prints(boot);
	//putc('1');
  /* read blk#2 to get group descriptor 0 */
  getblk((u16)2, buf1);
	//putc('2');
  gp = (GD *)buf1;
  iblk = (u16)gp->bg_inode_table; // typecast u32 to u16
	//get root inode
	getblk((u16)iblk,buf1);

	ip = (INODE *)buf1 + 1;
	
	//get first i_block of root inode
	getblk((u16)ip->i_block[0],buf2);
	//putc('2');
	inum = ino(buf2,"boot");
	//putc('1');
	//putc(inum+'0');
	//dp = (DIR *)buf2;
	//buf = buf2;
	
/*	while(buf < &buf2[BLK]){
			strncpy(temp, dp->name, dp->name_len);
			temp[dp->name_len]=0;
		//	prints("\nDir Name: ");prints(temp);prints("\n"); putc(dp->inode+'0');getc();
			if(strcmp(temp, "boot")==0){
				inum =(u16)dp->inode;
				break;
				}
				//
				//putc(iblk+'0');getc();
				//prints(" Found boot\n");
				
				
				
			buf+=dp->rec_len;
			dp = (DIR *)buf;
			//putc(dp->name_len+'0');
			}*/
	

/*get MTX Inode*/
/***************/

		//prints("\n Continued to find MTX\n");
		getblk((u16)(((inum-1)/8)+5),buf1);
		ip = (INODE *)buf1+(inum-1)%8;
//get the mtx inode to then load that block
		getblk((u16)ip->i_block[0],buf2);
		inum = ino(buf2,boot);
		putc(inum+'0');
	/*	dp = (DIR *)buf2;
		buf = buf2;
		//strncpy(temp, dp->name, dp->name_len);
		//prints("Dir Name: ");prints(temp);prints("\n");
		while(buf < &buf2[BLK]){
			strncpy(temp, dp->name, dp->name_len);
			temp[dp->name_len]=0;
			//prints("Dir Name: ");prints(temp);prints("\n");getc();
			if(strcmp(temp, boot)==0){
				inum = (u16)dp->inode;
				//putc(inum+'0');
				//prints("Found mtx\n");
				break;
				}
			buf+=dp->rec_len;
			dp = (DIR *)buf;
		}*/

		getblk((u16)(((inum-1)/8)+5),buf1);
		ip = (INODE *)buf1 + (inum-1)%8;
		getblk((u16)ip->i_block[12],buf2);
		//strncpy(temp, dp->name, dp->name_len);
		//temp[dp->name_len]=0;
		setes(0x1000);   // ES now points at segment 0x1000
		//
      for (i=0; i<12; i++){
           getblk((u16)ip->i_block[i], 0);  // load block to (ES, 0)
           inces();     // increment ES by 1KB (in 16-byte units)
						putc('1');
      }
	//	putc('0');
		ib = buf2;
		
		for(i=0;i<256;i++){
			//putc('0');
			getblk(*ib,0);
			putc('0');
			inces();
			putc('0');
			ib++;
			putc('2');
			}
	//prints("Going");
	return 1;

			
		
	
			

		//prints("\n\rAll done\n\r");
}  
