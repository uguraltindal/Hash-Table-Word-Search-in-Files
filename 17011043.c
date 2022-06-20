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
	char document[100]; 	// Kelimenin bulunduðu dökümanlar
	int counter;			// Load Factor program acilip kapaninca kaldigi yerden devam edecegi icin hash_table struct'ý içerisinde tanýmladým.
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
void dokuman(struct hash_table *array,char *dosyaAdi,int index)  // Örneðin case kelimesi farklý dosyalarda bulunuyor ise bunu kontrol eden ve struct içerisindeki döküman kýsmýna yazan fonksiyon
{
	int i,j;
	int set=0;
	int size1=strlen(dosyaAdi);
	int size2=strlen(array[index].document);     // Basit bir brute force karsilastirmasi yapacagimiz için String'lerin uzunluklarina ihtiyacýmýz var.
	for(i=0;i<=size2-size1;i++)
	{
		int eslesme = 0;						
		for(j=0;j<size1;j++)
		{
			if(array[index].document[i+j]==dosyaAdi[j])				// Özet olarak case kelimesi için dök1 kayýtlý iken dök2'den case tekrar gelirse.
			{														// dök1'in arkasýna dök2'yi yazdýrdýk.
				eslesme++;											// case kelimesi için dök1 tekrar alýnýrsa dök1 zaten mevcut olduðu için hiçbir yazdýrma iþlemi yapmadýk
			}
			else
			{
				j=size1+2;
			}
		}
		if(eslesme==size1)			// Eðer eslesme saðlandiysa örneðin kullanýcý döküman1'i 2 kere girerse case kelimesinin index'ine 2 kere döküman yazmamasý için
		{							// Gerekli kontrolü yaptýðýmýz kýsým. ÖRN: case dökuman1'de var mý? VAR = (Döküman ismini tekrar yazma)
			set=1;					
		}	
	}
	if(set==0)
	{
		strcat(array[index].document,"  ");
		strcat(array[index].document,dosyaAdi);	// ÖRN : case döküman 2'den tekrar alýndý. case döküman1 ve döküman2 eþleþmedi yani döküman 2 YOK = (döküman1'in arkasýna döküman2'yi ekle.
	}
}				

void insert(char kelime[30], struct hash_table *array,char *dosyaAdi,int *count)
{
	int key=findKey(kelime); // insert yaparken sadece kelimeyi buraya yollayýp Key degerini findKey fonksiyonunda stringi int'e dönüþtürerek çaðýrdýk.
	int i=0,j;
	int h1=hash1(key);
	int h2=hash2(key);					// hash islemeri icin degerleri h1 h2'ye atadik.
	int index=hash(h1,h2,i);			// Cunku ana hash fonksiyonumuz hash1 ve hash2'yi h1-h2 olarak kullanýyor.
	int sizekelime=strlen(kelime);
	printf("\n");
	if(*count==M)						// Eklenen kelime sayisi M(tablo boyutunu) astigi zaman uyarý verdik ve çýkýþ yaptýk.
	{
		printf("\n\n   Hash Table is full. No more elements can be added!   ");
		printf("\n\n 				Program is closing...");
		exit(-1);
	}
	int temp;
	while (array[index].flag==1)
	{
		temp=strcmp(array[index].string,kelime);				// Kelimelerin keyleri denk gelebilir.Fakat biz 2.bir kontrol daha yaptýrdýk.
		if(array[index].key==key && temp==0)					// string(x) Key degeri ile string(y) Key degeri ayný olsa bile
		{														// Stringlerin ayný olup olmadýðýný ekstra kontrol ederek farklý stringlerden gelebilecek
			printf(" %s  -> The word is already in the file. \n",kelime);
			dokuman(array,dosyaAdi,index);						// Olasý ayný Key deðerleri için fonksiyonumuzu doðru çalýþýr hale getirdik.		
			return;
		}
		i++;
		index=hash(h1,h2,i);									// Boþ yer bulana kadar i'yi arttýra arttýra hash'i devam ettirdik
	}

	array[index].flag=1;										// Bulunulan index'in flag==0 olmasý durumu orasýnýn boþ olduðunu gösterir.
	array[index].key=key;										// Böylece key dökuman ve string'i struct'ýn o indexindeki deðerlere atadýk.
	strcpy(array[index].document,dosyaAdi);						// Ayrýca o index'in flagini=1 yaparak orasýnýn artýk dolu olduðunu da belirttik.
	for(j=0;j<sizekelime;j++)
	{
		array[index].string[j]=kelime[j];
	}
	(*count)++;				// Pointer kullanarak count'u void içerisinde arttýrýp dýþarýda kullanabilecek hale getirdik.
}
void search(char aranankelime[30],struct hash_table *array)
{
	int key=findKey(aranankelime); 				// Aranan kelimenin keyini elde ettik.
	int i=0;
	int h1=hash1(key);
	int h2=hash2(key);
	int index=hash(h1,h2,i);
	int adimsayisi=1;							// Kac adýmda bulduðumuzu saydýrmak için tuttuk.
	int temp;									// temp degerini insert fonksiyonundaki gibi kelimelerin uyusup uyusmadigini anlamada kullandýk.
	while (array[index].flag==1)				// temp degeri eger 0'a eþit ise strcmp'un çalýþma mantýðýna göre kelimeler aynýdýr demektir.
	{
		temp=strcmp(array[index].string,aranankelime);				// buradaki islemler insert fonksiyonundaki islemlerle benzer.
		if(array[index].key==key && temp==0 )
		{
			printf("\n  Word : %s   \n\n  Files Found : %s  \n\n  Number of Steps Found : %d \n",aranankelime,array[index].document,adimsayisi);
			return;
		}
		i++;
		index=hash(h1,h2,i);							// Kodu ilk yazarken farklý kelimelerin ayný Key degerine denk gelmesi sebebiyle kelime kaybý
		adimsayisi++;									// Ve yanlýþ bilgi gösterimini farketmiþtim.strcmp kontrolünü if içerisinde ekstra tanýmlayarak bunu çözmüþ oldum.
						
	}													// yani artýk KEY DEGERLERÝ ayný olsa bile STRING eslesmez ise farklý kelimeler olduðunu farkediyoruz.
	
	if(array[index].key!=key)
	{
		printf("\n\n ARADIGINIZ KELIME DOSYA ICERISINDE BULUNMAMAKTADIR...");
	}
}
int findKey(char *str)									// Hashing hesaplamasýnda kullanýlacak Key degerinin olusturuldugu fonksiyon
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
	array[M].flag=0;											// Tüm flagleri 0'a setledim. Kullanýmýma göre flag=0 ise o index bos demektir.									// Yani tüm indexlerin bos oldugunu belirttim.
	char document_text[30000];
	char dosyaAdi[30]; 
	int j;
	int count=0;										// Count'u (LOAD FACTOR HESABINDA KULLANDIK) 
	array[0].counter=0;									// Ýlk basta counter'a 0 degerini atamamýz sadece ilk çalýþtýrma için gerekli.
	char aranankelime[30]; 								// Onun haricinde zaten LOAD FACTOR degerini dosyaya kaybettiðimiz ve
	char *tempstr;										// Bu LoadFactor'u her döküman eklemesi ya da programýn açýlýp kapanmasý sonucu
	int secim1;											// HashTable.txt üzerinden READ'leyerek degerleri çektigimiz için her zaman güncel degerine sahibiz.
	char secim2;
	FILE *dosya;
	
	dosya= fopen("HashTable.txt", "rb");				// Program acilip kapaninca yapacaðýmýz READ için direkt iþlemlerden önce 1 kere yazdýk.
	if (dosya != NULL) 
	{
    	fread(array, sizeof(array), 1, dosya);
    	fclose(dosya);
	}
	if(array[0].counter!=0)								// Deger 0'dan farklý ise güncel degeri program acýlýp kapansa bile count içerisine aktarýr.
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
		    
 			scanf("%s",dosyaAdi);			// eklemek istenilen dokumanýn ismi girilir.
 			
 			dosya = fopen(dosyaAdi,"r"); 	// Dokumandaki satýrlarý çeker ( ENTER ) basýlana kadar
			if(dosya!=NULL)					// Baþtan böyle baþladým hocam. Text dosyasý sýralý bir þekilde girildiði sürece tüm deðerleri eksiksiz okuyorum.
			{								// Fakat sürekli enter basýlarak girilen text'leri okuyamýyor.Sonradan deðiþtirecekti ama her þeyi deðiþtirmem gerekcekti
				fgets(document_text,30000,dosya);			// Atacaðým örnek görüntülerden programýn eksiksiz ve düzgün çalýþtýðýný göreceksiniz.
				strlwr(document_text);
				printf("\n\n Information has been successfully retrieved from the document.. ");
			}
			else
			{
				printf("\n Invalid filename ");
			}
			fclose(dosya);
			int textsize=strlen(document_text);		// Dokumandaki tüm string üzerinden kelime kelime ayýrma yaptýk.
			int j=0;
			while(j<textsize)
			{
			
				tempstr=(char*)malloc(30*sizeof(char));
				int k=0;
				for(j;97<=((int)document_text[j]) && ((int)document_text[j])<=122;j++)	// Sozluk oldugu için kelimelerin harflerinin küçük olmasýyla birlikte
				{
					tempstr[k]=document_text[j];										// Küçük harf harici deðer görürsen dedim çünkü bosluk harici / gibi iþaretler
					k++;																// Görmesi durumunda onlarý da ekleyebilirdik.
				}																		// Belki ödevde noktalama iþaretleri yok denmiþ ama ben daha kapsamlý olacaðý için bunu tercih ettim.
				j++;
				
				insert(tempstr,array,dosyaAdi,&count);		// Kelimeyi ayýrdýkta insert fonksiyonuna yolladýk ve gerekli kontrolleri yaptýrdýk.
				printf("\n\n Count : %d",count);
				free(tempstr);
				array[0].counter=count;						// Write ile HashTable.txt'ye kaydedeceðimiz için LOAD FACTOR deðerini HashTable.txt'ye atadýk.
				float temp=count;
				float loadFactor= temp/M;					// LoadFactor hesabý yapýlan kýsým.
				static int uyari=0; 						// Uyarýnýn 1 kere gözükmesi için static tanýmladým.
				if(loadFactor>0.8 && uyari==0)				
				{
					printf("\n\n\n			(W   A   R   N   I   N   G) !!!  \n\n\n");		// LoadFactor degeri 0.8'i gecince UYARI bastýrdým.	
					printf("\n\n PLEASE WAIT.... THE PROGRAM WAS SLEEPED FOR 5 SECONDS SO THAT THE USER DOESN'T MISS THE WARNING\n\n");
					sleep(5);														// Kelime yüklemesi çok hýzlý olduðu için kullanýcýnýn rahat görmesi adýna sleep fonksiyonu ile4 saniye uyuttum.
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
			
			dosya= fopen("HashTable.txt", "wb");				// Caseden çýkmadan önce tüm bilgileri hashtable.txt'ye atayarak kaydettik.Artýk programý kapatýp açsak da güncel olarak devam edebiliriz.
			if (dosya != NULL) 
			{
				printf("\n\n  							Hash table has been created successfully...\n");
    			fwrite(array, sizeof(array), 1, dosya);			// Hash dosyamýza yazdýk.
    			fclose(dosya);
			}
			int i;	
		    break;
 
   		case 2:									// Search yapýlan CASE2
 
		      printf("\n Enter the word you want to search :  ");
		      scanf("%s",aranankelime);
		      strlwr(aranankelime);				// Sozluk olduðu için kelimleri kuculterek arama yaptým.
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
