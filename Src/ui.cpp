#include "ui.h"
#include "version.h"

namespace UI {
    void PrintUsage() {
        std::wcout << Version::PROGRAM_NAME << std::endl;
        std::wcout << L"==========================================" << std::endl;
        std::wcout << L"Usage: " << Version::PROGRAM_NAME << L" <MSG_FILE_PATH> [OUTPUT_DIR]" << std::endl;
        std::wcout << std::endl;
        std::wcout << L"Arguments:" << std::endl;
        std::wcout << L"  MSG_FILE_PATH    Path to the MSG file to analyze" << std::endl;
        std::wcout << L"  OUTPUT_DIR       Optional output directory (default: auto-generated)" << std::endl;
        std::wcout << std::endl;
        std::wcout << L"Examples:" << std::endl;
        std::wcout << L"  " << Version::PROGRAM_NAME << L" C:\\test.msg" << std::endl;
        std::wcout << L"  " << Version::PROGRAM_NAME << L" test.msg C:\\output" << std::endl;
        std::wcout << std::endl;
        std::wcout << L"Features:" << std::endl;
        std::wcout << L"  - Recursively traverse all Storage/Stream in MSG file" << std::endl;
        std::wcout << L"  - Convert Stream data to hex format and save" << std::endl;
        std::wcout << L"  - Save Storage as folders, Stream as .dat files" << std::endl;
        std::wcout << L"  - Parse MAPI properties including Recipient types (To, Cc, Bcc)" << std::endl;
        std::wcout << L"  - Auto-generate output directory with timestamp" << std::endl;
        std::wcout << std::endl;
        std::wcout << L"Output Directory Format:" << std::endl;
        std::wcout << L"  When no output directory is specified, creates:" << std::endl;
        std::wcout << L"  YYYYMMDD_HHMMSS_filename_data" << std::endl;
        std::wcout << std::endl;
        std::wcout << L"Note: This program requires Administrator privileges to access MSG files." << std::endl;
        std::wcout << L"Please run as Administrator if you encounter access denied errors." << std::endl;
    }

} // namespace UI
