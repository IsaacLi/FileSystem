#include "File.h"
using namespace std;

const int ROOT_HEADER_SIZE = 119;
/* Hardware representation of the root directory, used to write to disc. */

/* Software Declarations for when the root directory is in memory	*/

class FileNode {
	public:
		char type; //0表普通文件，1表目录
		string fileName;
		blockNum fileLoc;
		int fileSize;
		FileNode *pNext;
		/*	Constructor	*/
		FileNode(string name, int size, blockNum location) : fileName(name), 
		fileSize(size), fileLoc(location), pNext(NULL), type(0) {  }
		void Print();
};

class Dir {
	private:	
		int discID;
		FileHeader rootHeader;
		unsigned short count;
		blockNum headerLoc;
	public:
		FileNode *pHead;
		Dir();
		Dir(blockNum headerLocation, int discIDNum);
		~Dir() {}
		void setDiscID (int idNum) { this->discID = idNum; }
		unsigned short GetCount() const {return this->count;}
		bool AddFile (string name, int size, blockNum location);
		bool AddFile (blockNum location);
		bool AddDir(string name, blockNum location);
		bool AddDir(blockNum location);
		bool RemoveDir(string dirToRemove);
		bool RemoveFile (string fileToRemove);
		bool SaveRoot ();
		bool LoadRoot ();
		void Print() const;
		blockNum FindFile(string searchKey) const;
		blockNum FindFile(blockNum searchKey) const;
		string GetName() { return this->rootHeader.name; }
		void SetName(string DirName) { strcpy(this->rootHeader.name, DirName.c_str()); }
		int GetSize() { return this->rootHeader.size; };
		int GetHeaderLoc() { return this->headerLoc; };
		void ClearFileNode();
};

