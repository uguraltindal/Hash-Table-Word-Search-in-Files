#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#define M 997 
// HASH TABLE ODEVI

struct hash_table
{
	int key;
	int flag;				// Bayrak 1 ise o index dolu demektir.Bayrak bos ise o index 0 demektir.
	char string[100];		// O index'te bulunan Key'e ait kelime
	char document[100]; 	// Kelimenin bulundu�u d�k�manlar
	int counter;			// Load Factor program acilip kapaninca kaldigi yerden devam edecegi icin hash_table struct'� i�erisinde tan�mlad�m.
};
int hash(int h1,int h2,int i)	// Hashing
{
	return (h1+(i*h2))%M;
}
int hash1(int key)				// Hash1
{
	return key % M;
}
int hash2(int key)				// Hash2
{
	return 1+(key%(M-1));
}
void dokuman(struct hash_table *array,char *dosyaAdi,int index)  // �rne�in case kelimesi farkl� dosyalarda bulunuyor ise bunu kontrol eden ve struct i�erisindeki d�k�man k�sm�na yazan fonksiyon
{
	int i,j;
	int set=0;
	int size1=strlen(dosyaAdi);
	int size2=strlen(array[index].document);     // Basit bir brute force karsilastirmasi yapacagimiz i�in String'lerin uzunluklarina ihtiyac�m�z var.
	for(i=0;i<=size2-size1;i++)
	{
		int eslesme = 0;						
		for(j=0;j<size1;j++)
		{
			if(array[index].document[i+j]==dosyaAdi[j])				// �zet olarak case kelimesi i�in d�k1 kay�tl� iken d�k2'den case tekrar gelirse.
			{														// d�k1'in arkas�na d�k2'yi yazd�rd�k.
				eslesme++;											// case kelimesi i�in d�k1 tekrar al�n�rsa d�k1 zaten mevcut oldu�u i�in hi�bir yazd�rma i�lemi yapmad�k
			}
			else
			{
				j=size1+2;
			}
		}
		if(eslesme==size1)			// E�er eslesme sa�landiysa �rne�in kullan�c� d�k�man1'i 2 kere girerse case kelimesinin index'ine 2 kere d�k�man yazmamas� i�in
		{							// Gerekli kontrol� yapt���m�z k�s�m. �RN: case d�kuman1'de var m�? VAR = (D�k�man ismini tekrar yazma)
			set=1;					
		}	
	}
	if(set==0)
	{
		strcat(array[index].document,"  ");
		strcat(array[index].document,dosyaAdi);	// �RN : case d�k�man 2'den tekrar al�nd�. case d�k�man1 ve d�k�man2 e�le�medi yani d�k�man 2 YOK = (d�k�man1'in arkas�na d�k�man2'yi ekle.
	}
}				

void insert(char kelime[30], struct hash_table *array,char *dosyaAdi,int *count)
{
	int key=findKey(kelime); // insert yaparken sadece kelimeyi buraya yollay�p Key degerini findKey fonksiyonunda stringi int'e d�n��t�rerek �a��rd�k.
	int i=0,j;
	int h1=hash1(key);
	int h2=hash2(key);					// hash islemeri icin degerleri h1 h2'ye atadik.
	int index=hash(h1,h2,i);			// Cunku ana hash fonksiyonumuz hash1 ve hash2'yi h1-h2 olarak kullan�yor.
	int sizekelime=strlen(kelime);
	printf("\n");
	if(*count==M)						// Eklenen kelime sayisi M(tablo boyutunu) astigi zaman uyar� verdik ve ��k�� yapt�k.
	{
		printf("\n\n   Hash Table is full. No more elements can be added!   ");
		printf("\n\n 				Program is closing...");
		exit(-1);
	}
	int temp;
	while (array[index].flag==1)
	{
		temp=strcmp(array[index].string,kelime);				// Kelimelerin keyleri denk gelebilir.Fakat biz 2.bir kontrol daha yapt�rd�k.
		if(array[index].key==key && temp==0)					// string(x) Key degeri ile string(y) Key degeri ayn� olsa bile
		{														// Stringlerin ayn� olup olmad���n� ekstra kontrol ederek farkl� stringlerden gelebilecek
			printf(" %s  -> The word is already in the file. \n",kelime);
			dokuman(array,dosyaAdi,index);						// Olas� ayn� Key de�erleri i�in fonksiyonumuzu do�ru �al���r hale getirdik.		
			return;
		}
		i++;
		index=hash(h1,h2,i);									// Bo� yer bulana kadar i'yi artt�ra artt�ra hash'i devam ettirdik
	}

	array[index].flag=1;										// Bulunulan index'in flag==0 olmas� durumu oras�n�n bo� oldu�unu g�sterir.
	array[index].key=key;										// B�ylece key d�kuman ve string'i struct'�n o indexindeki de�erlere atad�k.
	strcpy(array[index].document,dosyaAdi);						// Ayr�ca o index'in flagini=1 yaparak oras�n�n art�k dolu oldu�unu da belirttik.
	for(j=0;j<sizekelime;j++)
	{
		array[index].string[j]=kelime[j];
	}
	(*count)++;				// Pointer kullanarak count'u void i�erisinde artt�r�p d��ar�da kullanabilecek hale getirdik.
}
void search(char aranankelime[30],struct hash_table *array)
{
	int key=findKey(aranankelime); 				// Aranan kelimenin keyini elde ettik.
	int i=0;
	int h1=hash1(key);
	int h2=hash2(key);
	int index=hash(h1,h2,i);
	int adimsayisi=1;							// Kac ad�mda buldu�umuzu sayd�rmak i�in tuttuk.
	int temp;									// temp degerini insert fonksiyonundaki gibi kelimelerin uyusup uyusmadigini anlamada kulland�k.
	while (array[index].flag==1)				// temp degeri eger 0'a e�it ise strcmp'un �al��ma mant���na g�re kelimeler ayn�d�r demektir.
	{
		temp=strcmp(array[index].string,aranankelime);				// buradaki islemler insert fonksiyonundaki islemlerle benzer.
		if(array[index].key==key && temp==0 )
		{
			printf("\n  Word : %s   \n\n  Files Found : %s  \n\n  Number of Steps Found : %d \n",aranankelime,array[index].document,adimsayisi);
			return;
		}
		i++;
		index=hash(h1,h2,i);							// Kodu ilk yazarken farkl� kelimelerin ayn� Key degerine denk gelmesi sebebiyle kelime kayb�
		adimsayisi++;									// Ve yanl�� bilgi g�sterimini farketmi�tim.strcmp kontrol�n� if i�erisinde ekstra tan�mlayarak bunu ��zm�� oldum.
						
	}													// yani art�k KEY DEGERLER� ayn� olsa bile STRING eslesmez ise farkl� kelimeler oldu�unu farkediyoruz.
	
	if(array[index].key!=key)
	{
		printf("\n\n ARADIGINIZ KELIME DOSYA ICERISINDE BULUNMAMAKTADIR...");
	}
}
int findKey(char *str)									// Hashing hesaplamas�nda kullan�lacak Key degerinin olusturuldugu fonksiyon
{
				
	int sum=0,j;		
	for(j=0;97<=((int)str[j]) && ((int)str[j])<=122;j++)		// 						HORNER METODU
	{
		sum+=str[j]*pow(3,j);;
	}
	return sum;										 
}

int main()
{
	struct hash_table array[M];
	array[M].flag=0;											// T�m flagleri 0'a setledim. Kullan�m�ma g�re flag=0 ise o index bos demektir.									// Yani t�m indexlerin bos oldugunu belirttim.
	char document_text[30000];
	char dosyaAdi[30]; 
	int j;
	int count=0;										// Count'u (LOAD FACTOR HESABINDA KULLANDIK) 
	array[0].counter=0;									// �lk basta counter'a 0 degerini atamam�z sadece ilk �al��t�rma i�in gerekli.
	char aranankelime[30]; 								// Onun haricinde zaten LOAD FACTOR degerini dosyaya kaybetti�imiz ve
	char *tempstr;										// Bu LoadFactor'u her d�k�man eklemesi ya da program�n a��l�p kapanmas� sonucu
	int secim1;											// HashTable.txt �zerinden READ'leyerek degerleri �ektigimiz i�in her zaman g�ncel degerine sahibiz.
	char secim2;
	FILE *dosya;
	
	dosya= fopen("HashTable.txt", "rb");				// Program acilip kapaninca yapaca��m�z READ i�in direkt i�lemlerden �nce 1 kere yazd�k.
	if (dosya != NULL) 
	{
    	fread(array, sizeof(array), 1, dosya);
    	fclose(dosya);
	}
	if(array[0].counter!=0)								// Deger 0'dan farkl� ise g�ncel degeri program ac�l�p kapansa bile count i�erisine aktar�r.
	{
		count=array[0].counter;
	}
	do {
		
		printf("\n\n    			MENU \n\n    1.To add a document to Hash Table : 1" 
               	"\n\n    2.To search words in Hash Table : 2"  
		       );
 		printf("		Choice : ");
 		scanf("%d", &secim1);
 		printf("\n-------------------------------------------------------------------------------------------------------");
		switch(secim1) 
        {
 
		case 1:								// Dokuman ekleme CASE 1
 
		    printf("\n\n    Enter the name of the document you want to add as .txt (Ex : deneme.txt) : ");
		    
 			scanf("%s",dosyaAdi);			// eklemek istenilen dokuman�n ismi girilir.
 			
 			dosya = fopen(dosyaAdi,"r"); 	// Dokumandaki sat�rlar� �eker ( ENTER ) bas�lana kadar
			if(dosya!=NULL)					// Ba�tan b�yle ba�lad�m hocam. Text dosyas� s�ral� bir �ekilde girildi�i s�rece t�m de�erleri eksiksiz okuyorum.
			{								// Fakat s�rekli enter bas�larak girilen text'leri okuyam�yor.Sonradan de�i�tirecekti ama her �eyi de�i�tirmem gerekcekti
				fgets(document_text,30000,dosya);			// Ataca��m �rnek g�r�nt�lerden program�n eksiksiz ve d�zg�n �al��t���n� g�receksiniz.
				strlwr(document_text);
				printf("\n\n Information has been successfully retrieved from the document.. ");
			}
			else
			{
				printf("\n Invalid filename ");
			}
			fclose(dosya);
			int textsize=strlen(document_text);		// Dokumandaki t�m string �zerinden kelime kelime ay�rma yapt�k.
			int j=0;
			while(j<textsize)
			{
			
				tempstr=(char*)malloc(30*sizeof(char));
				int k=0;
				for(j;97<=((int)document_text[j]) && ((int)document_text[j])<=122;j++)	// Sozluk oldugu i�in kelimelerin harflerinin k���k olmas�yla birlikte
				{
					tempstr[k]=document_text[j];										// K���k harf harici de�er g�r�rsen dedim ��nk� bosluk harici / gibi i�aretler
					k++;																// G�rmesi durumunda onlar� da ekleyebilirdik.
				}																		// Belki �devde noktalama i�aretleri yok denmi� ama ben daha kapsaml� olaca�� i�in bunu tercih ettim.
				j++;
				
				insert(tempstr,array,dosyaAdi,&count);		// Kelimeyi ay�rd�kta insert fonksiyonuna yollad�k ve gerekli kontrolleri yapt�rd�k.
				printf("\n\n Count : %d",count);
				free(tempstr);
				array[0].counter=count;						// Write ile HashTable.txt'ye kaydedece�imiz i�in LOAD FACTOR de�erini HashTable.txt'ye atad�k.
				float temp=count;
				float loadFactor= temp/M;					// LoadFactor hesab� yap�lan k�s�m.
				static int uyari=0; 						// Uyar�n�n 1 kere g�z�kmesi i�in static tan�mlad�m.
				if(loadFactor>0.8 && uyari==0)				
				{
					printf("\n\n\n			(W   A   R   N   I   N   G) !!!  \n\n\n");		// LoadFactor degeri 0.8'i gecince UYARI bast�rd�m.	
					printf("\n\n PLEASE WAIT.... THE PROGRAM WAS SLEEPED FOR 5 SECONDS SO THAT THE USER DOESN'T MISS THE WARNING\n\n");
					sleep(5);														// Kelime y�klemesi �ok h�zl� oldu�u i�in kullan�c�n�n rahat g�rmesi ad�na sleep fonksiyonu ile4 saniye uyuttum.
					printf("   LoadFactor value : %0.4f",loadFactor);
					printf("   W A R N I N G  :   LoadFactor value is over 0.8..");
					uyari=1;
					sleep(5);					
				}
				else
				{
					printf(" 	 LoadFactor value : %0.4f",loadFactor);
				}
			}
			
			dosya= fopen("HashTable.txt", "wb");				// Caseden ��kmadan �nce t�m bilgileri hashtable.txt'ye atayarak kaydettik.Art�k program� kapat�p a�sak da g�ncel olarak devam edebiliriz.
			if (dosya != NULL) 
			{
				printf("\n\n  							Hash table has been created successfully...\n");
    			fwrite(array, sizeof(array), 1, dosya);			// Hash dosyam�za yazd�k.
    			fclose(dosya);
			}
			int i;	
		    break;
 
   		case 2:									// Search yap�lan CASE2
 
		      printf("\n Enter the word you want to search :  ");
		      scanf("%s",aranankelime);
		      strlwr(aranankelime);				// Sozluk oldu�u i�in kelimleri kuculterek arama yapt�m.
		      search(aranankelime,array);
				
			  break;
			  
			  default:
 
		       printf("\n You made the wrong choice \n");
 
		}
 		
		printf("\n\n\n  Menu  (Continue : D   --   Exit : C )     Choice :    ");
		scanf(" %c",&secim2);
 
	}while(secim2 == 'D' || secim2 == 'd');
	
	
	return 0;
}
