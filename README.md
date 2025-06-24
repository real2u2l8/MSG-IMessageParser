# MSG File Parser Tool (iMessage Parser)

## ����
**MSG File Parser Tool**�� Microsoft Outlook�� `.msg` ������ �м��Ͽ� ���� ������ �����͸� �����ϴ� ��� �����Դϴ�. MAPI(Messaging Application Programming Interface) �Ӽ��� �Ľ��ϰ�, ������ ��� ���� ��Ҹ� ���� �м��� �� �ֽ��ϴ�.

## �ֿ� ���

### MSG ���� ���� �м�
- **����� Ž��**: ��� Storage/Stream ������ ������ Ž��
- **���� ���� ����**: Storage�� ������, Stream�� `.dat` ���Ϸ� ����
- **���̳ʸ� ������ ����**: ��� ��Ʈ�� �����͸� ���� ���̳ʸ� ���·� ����

### MAPI �Ӽ� �Ľ�
- **���� �⺻ ����**: �߽���, ������(To/Cc/Bcc), ����, ����, ���
- **������ �м�**: �� �������� Ÿ�԰� �̸��� �ּ� �� �м�
- **÷������ ����**: ÷�����ϸ�, ǥ�ø�, ������ ����
- **�Ӽ� ���̺�**: ��� MAPI �Ӽ��� ����ȭ�Ͽ� ����

### ��� ���
- **�ڵ� ���丮 ����**: Ÿ�ӽ����� ��� �ڵ� ��� ���丮 ����
- **������ ���� Ȯ��**: MSG ���� ������ ���� ���� ����
- **�� �α�**: ó�� ������ ��� �ܰ踦 �α׷� ���
- **���� ó��**: ������ ���ϸ� ���� �� ���� ��Ȳ ó��

## ��� ���

### �⺻ ����
```bash
# �⺻ ��� (�ڵ� ��� ���丮 ����)
IMessageParser.exe "C:\path\to\message.msg"

# ����� ���� ��� ���丮
IMessageParser.exe "C:\path\to\message.msg" "C:\output\directory"
```

### ����� ����
- **MSG_FILE_PATH** (�ʼ�): �м��� MSG ������ ���
- **OUTPUT_DIR** (����): ��� ���� ���丮 (������ �� �ڵ� ����)

### ��� ���丮 ����
�ڵ� �����Ǵ� ���丮��: `YYYYMMDD_HHMMSS_filename_data`

����: `20241201_143052_test_message_data`

## ��� ����

```
output_directory/
������ __nameid_version1.0/          # Named Property ���� ����
������ __recip_version1.0_#00000000/ # ������ ���̺� ���� (ù ��° ������)
������ __recip_version1.0_#00000001/ # ������ ���̺� ���� (�� ��° ������)
������ __recip_version1.0_#00000002/ # ������ ���̺� ���� (�� ��° ������)
������ __properties_version1.0.dat   # ���� �Ӽ� ����
������ __substg1.0_0C1F001F.dat      # �߽��� �̸��� �ּ�
������ __substg1.0_0E04001F.dat      # ������(To) ����
������ __substg1.0_0E03001F.dat      # ����(Cc) ����
������ __substg1.0_0E02001F.dat      # ��������(Bcc) ����
������ __substg1.0_0037001F.dat      # ���� ����
������ __substg1.0_1000001F.dat      # ���� ����
������ __substg1.0_0070001F.dat      # ���� ���λ� (RE:, FW: ��)
������ ... (��Ÿ ��� ��Ʈ��)
```

## �ý��� �䱸����

### �ʼ� ����
- **Windows 10/11** (x64 ����)
- **������ ����**: MSG ���� ������ ���� �ʿ�
- **Visual Studio 2017** �̻� (���� ��)

### ������
- **Windows API**: COM, IStorage, IStream
- **MAPI ���̺귯��**: ���� ���� ó��
- **C++17** ǥ��

## ���� ���

### Visual Studio 2017���� ����
1. `IMessageParser.sln` ������ Visual Studio 2017���� ����
2. �÷����� **x64**�� ����
3. ������ **Release** �Ǵ� **Debug** ����
4. **����** �� **�ַ�� ����**

### ����� ����
```bash
msbuild Src/IMessageParser.sln /p:Configuration=Release /p:Platform=x64
```

## �м� ������ ������

### ���� �⺻ ����
- **�߽���**: �̸��� �ּ�, �ּ� Ÿ�� (SMTP/EX)
- **������**: To, Cc, Bcc ���� �� �� ����
- **����**: ���� ���� �� ���λ� (RE:, FW: ��)
- **����**: �ؽ�Ʈ ������ ���� ����

### ÷������ ����
- **���ϸ�**: ���� ���ϸ� �� ǥ�ø�
- **������**: ���� ÷������ ���̳ʸ� ������
- **�Ӽ�**: ÷������ ���� ��� MAPI �Ӽ�

### MAPI �Ӽ�
- **Property Tag**: �Ӽ� ID�� Ÿ�� ����
- **������ Ÿ��**: ���ڿ�, �����ڵ�, ���̳ʸ�, ���� ��
- **�� �ؼ�**: �� ������ Ÿ�Կ� ���� ������ ��ȯ

## ����� ���λ���

### MSG ���� ����
MSG ������ Microsoft�� Compound File Binary Format�� ����ϸ�, ������ ���� ������ �����ϴ�:
- **Storage**: ������ ���� ���� ����
- **Stream**: ���� �����͸� �����ϴ� ���̳ʸ� ��Ʈ��
- **MAPI Properties**: ���� ������ �����ϴ� ����ȭ�� ������

### �Ľ� ���μ���
1. **COM �ʱ�ȭ**: IStorage �������̽� ����� ���� �ʱ�ȭ
2. **����� Ž��**: ��� Storage�� Stream�� ��ȸ
3. **������ ����**: �� Stream�� ���̳ʸ� ���·� ����
4. **�Ӽ� �Ľ�**: MAPI �Ӽ� ���̺� �м�
5. **��� ����**: ����ȭ�� ���·� ���� �ý��ۿ� ����

## ���� �ذ�

### �Ϲ����� ����
- **"Administrator privileges are required"**: ������ �������� ����
- **"Access denied"**: ������ �ٸ� ���α׷����� ��� ������ Ȯ��
- **"File not found"**: MSG ���� ��ΰ� �ùٸ��� Ȯ��

### �����
- **�� �α�**: ���α׷� ���� �� ��� ó�� ������ �ֿܼ� ���
- **���� �޽���**: ��ü���� ���� ���ΰ� �ذ� ��� ����
- **���� ��Ȳ**: ���� ó�� ������� �ǽð����� ǥ��

## ���̼���
�� ������Ʈ�� ���� �� ���� �������� ���ߵǾ����ϴ�.

## ������
- **������**: real2u2l8
- **����**: 1.0.0.1
- **����**: Advanced MSG file analysis tool with MAPI property parsing

## ���� �ڷ�
- [MS-OXMSG: Outlook Item (.msg) File Format](https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-oxmsg/621801cb-b617-474c-bce6-69037d73461a)
- [MAPI Property Tags](https://learn.microsoft.com/en-us/office/client-developer/outlook/mapi/mapi-property-tags)
- [Compound File Binary Format](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-cfb/53989ce4-7b05-4f8d-829d-d738d5b0131c)
