#include "privilege.h"

namespace Privilege {

    bool IsRunningAsAdministrator() {
        BOOL isAdmin = FALSE;
        PSID adminGroup = NULL;
        SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
        
        // Administrators 그룹의 SID 생성
        if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
            
            // 현재 프로세스가 Administrators 그룹에 속하는지 확인
            if (!CheckTokenMembership(NULL, adminGroup, &isAdmin)) {
                isAdmin = FALSE;
            }
            
            FreeSid(adminGroup);
        }
        
        return isAdmin == TRUE;
    }

} // namespace Privilege
