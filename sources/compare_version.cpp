// 说明
// Compare two version numbers version1 and version1.
// If version1 > version2 return 1, if version1 < version2 return -1, otherwise return 0.
// You may assume that the version strings are non-empty and contain only digits and the . character. The . character does not represent a decimal point and is used to separate number sequences. For instance, 2.5 is not "two and a half" or "half way to version three", it is the fifth second-level revision of the second first-level revision.
// Here is an example of version numbers ordering:
// 0.1 < 1.1 < 1.2 < 13.37
int compareVersion(string version1, string version2) {
    int ver1, ver2;
    if (version1.empty() && version2.empty())
        return 0;
        
    size_t pos1 = version1.find(".");
    if (version1.empty()) {
        ver1 = 0;
    } else {
        if (pos1 == string::npos) {
            pos1 = version1.length();
        }
        ver1 = atoi(version1.substr(0, pos1).c_str());
    }
    
    size_t pos2 = version2.find(".");
    if (version2.empty()) {
        ver2 = 0;
    } else {
        if (pos2 == string::npos) {
            pos2 = version2.length();
        }
        ver2 = atoi(version2.substr(0, pos2).c_str());
    }
    
    if (ver1 > ver2)
        return 1;
    else if (ver1 < ver2)
        return -1;
    else // 这个递归的使用是精髓
        return compareVersion(pos1 == version1.length()? "" : version1.substr(pos1+1), pos2 == version2.length() ? "" : version2.substr(pos2+1));
}
