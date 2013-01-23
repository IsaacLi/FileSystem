#include "FileSys.h"
#include <limits>
using namespace std;

enum choice_t {
	Print_Menu,
	Create_Disc,
	Format_Disc,
	Mount_Disc,
	List_Root,
	Create_File,
	Update_File,
	Read_File,
	Delete_File,
	Open_Dir,
	Make_Dir,
	Exit
};

class UserInterface {
private:
	short int choice;
	short int GetChoice();
	bool ExecChoice (FileSys *fs);
	void PrintMenu () const;
public:
	UserInterface () {}
	~UserInterface () {}
	void run (FileSys *fs);	
};
