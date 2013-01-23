#include "File.h"
#include "Dir.h"
#include "FBQ.h"
using namespace std;

/* 	Hardware representation - used to save to disc.
	Stores info about the filesystem	*/
struct SuperBlock {
	char discName[32];
	int totalBlocks;
	int blocksFree;
	byte padding[472];
};

/*	Software representation for when FileSys is in memory	*/
class FileSys {
private:
	short int discID;
	SuperBlock FSHeader;
	FreeBlockQueue fbq; 
	Dir root;
	Dir currentDir;//当前所处的路径
	void GetSBInfo();
	void UpdateSuperBlock();
	File* OpenFile(blockNum fileHeaderLoc);
	void CloseFile(File* FileToClose);
public:
	FileSys () {this->discID = -1;}
	~FileSys () {this->discID = -1;}
	void SetDiscName (string newName);
	int GetDiscID () const {return this->discID;}
	void CreateDisc ();
	void ReformatDisc ();
	void MountDisc ();
	void ListCurrentDir() const {this->currentDir.Print();}	
	void CreateFile ();
	void PrintFile ();
	void UpdateFile ();
	void DeleteFile ();
	void Exit (); 
	void OpenDir(); //将 currentDir 指向 dirName
	void MkDir();  //在此dir 下，创建一个新的dir 挂在当前目录下
};
