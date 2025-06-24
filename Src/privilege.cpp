#include "privilege.h"

namespace Privilege {

    bool IsRunningAsAdministrator() {
        BOOL isAdmin = FALSE;
        PSID adminGroup = NULL;
        SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
        
        // Administrators �׷��� SID ����
        if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
            
            // ���� ���μ����� Administrators �׷쿡 ���ϴ��� Ȯ��
            if (!CheckTokenMembership(NULL, adminGroup, &isAdmin)) {
                isAdmin = FALSE;
            }
            
            FreeSid(adminGroup);
        }
        
        return isAdmin == TRUE;
    }

} // namespace Privilege
