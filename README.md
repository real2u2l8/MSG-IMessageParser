# MSG File Parser Tool (iMessage Parser)

## 개요
**MSG File Parser Tool**은 Microsoft Outlook의 `.msg` 파일을 분석하여 내부 구조와 데이터를 추출하는 고급 도구입니다. MAPI(Messaging Application Programming Interface) 속성을 파싱하고, 메일의 모든 구성 요소를 상세히 분석할 수 있습니다.

## 주요 기능

### MSG 파일 구조 분석
- **재귀적 탐색**: 모든 Storage/Stream 구조를 완전히 탐색
- **계층 구조 저장**: Storage는 폴더로, Stream은 `.dat` 파일로 저장
- **바이너리 데이터 저장**: 모든 스트림 데이터를 원본 바이너리 형태로 저장

### MAPI 속성 파싱
- **메일 기본 정보**: 발신자, 수신자(To/Cc/Bcc), 제목, 본문, 헤더
- **수신자 분석**: 각 수신자의 타입과 이메일 주소 상세 분석
- **첨부파일 정보**: 첨부파일명, 표시명, 데이터 추출
- **속성 테이블**: 모든 MAPI 속성을 구조화하여 저장

### 고급 기능
- **자동 디렉토리 생성**: 타임스탬프 기반 자동 출력 디렉토리 생성
- **관리자 권한 확인**: MSG 파일 접근을 위한 권한 검증
- **상세 로깅**: 처리 과정의 모든 단계를 로그로 기록
- **에러 처리**: 안전한 파일명 생성 및 예외 상황 처리

## 사용 방법

### 기본 사용법
```bash
# 기본 사용 (자동 출력 디렉토리 생성)
IMessageParser.exe "C:\path\to\message.msg"

# 사용자 지정 출력 디렉토리
IMessageParser.exe "C:\path\to\message.msg" "C:\output\directory"
```

### 명령행 인자
- **MSG_FILE_PATH** (필수): 분석할 MSG 파일의 경로
- **OUTPUT_DIR** (선택): 결과 저장 디렉토리 (미지정 시 자동 생성)

### 출력 디렉토리 형식
자동 생성되는 디렉토리명: `YYYYMMDD_HHMMSS_filename_data`

예시: `20241201_143052_test_message_data`

## 출력 구조

```
output_directory/
├── __nameid_version1.0/          # Named Property 매핑 정보
├── __recip_version1.0_#00000000/ # 수신자 테이블 정보 (첫 번째 수신자)
├── __recip_version1.0_#00000001/ # 수신자 테이블 정보 (두 번째 수신자)
├── __recip_version1.0_#00000002/ # 수신자 테이블 정보 (세 번째 수신자)
├── __properties_version1.0.dat   # 메일 속성 정보
├── __substg1.0_0C1F001F.dat      # 발신자 이메일 주소
├── __substg1.0_0E04001F.dat      # 수신자(To) 정보
├── __substg1.0_0E03001F.dat      # 참조(Cc) 정보
├── __substg1.0_0E02001F.dat      # 숨은참조(Bcc) 정보
├── __substg1.0_0037001F.dat      # 메일 제목
├── __substg1.0_1000001F.dat      # 메일 본문
├── __substg1.0_0070001F.dat      # 제목 접두사 (RE:, FW: 등)
└── ... (기타 모든 스트림)
```

## 시스템 요구사항

### 필수 조건
- **Windows 10/11** (x64 지원)
- **관리자 권한**: MSG 파일 접근을 위해 필요
- **Visual Studio 2017** 이상 (빌드 시)

### 의존성
- **Windows API**: COM, IStorage, IStream
- **MAPI 라이브러리**: 버전 정보 처리
- **C++17** 표준

## 빌드 방법

### Visual Studio 2017에서 빌드
1. `IMessageParser.sln` 파일을 Visual Studio 2017에서 열기
2. 플랫폼을 **x64**로 설정
3. 구성은 **Release** 또는 **Debug** 선택
4. **빌드** → **솔루션 빌드**

### 명령행 빌드
```bash
msbuild Src/IMessageParser.sln /p:Configuration=Release /p:Platform=x64
```

## 분석 가능한 데이터

### 메일 기본 정보
- **발신자**: 이메일 주소, 주소 타입 (SMTP/EX)
- **수신자**: To, Cc, Bcc 구분 및 상세 정보
- **제목**: 메일 제목 및 접두사 (RE:, FW: 등)
- **본문**: 텍스트 형태의 메일 내용

### 첨부파일 정보
- **파일명**: 원본 파일명 및 표시명
- **데이터**: 실제 첨부파일 바이너리 데이터
- **속성**: 첨부파일 관련 모든 MAPI 속성

### MAPI 속성
- **Property Tag**: 속성 ID와 타입 조합
- **데이터 타입**: 문자열, 유니코드, 바이너리, 정수 등
- **값 해석**: 각 데이터 타입에 따른 적절한 변환

## 기술적 세부사항

### MSG 파일 구조
MSG 파일은 Microsoft의 Compound File Binary Format을 사용하며, 다음과 같은 구조를 가집니다:
- **Storage**: 폴더와 같은 계층 구조
- **Stream**: 실제 데이터를 저장하는 바이너리 스트림
- **MAPI Properties**: 메일 정보를 저장하는 구조화된 데이터

### 파싱 프로세스
1. **COM 초기화**: IStorage 인터페이스 사용을 위한 초기화
2. **재귀적 탐색**: 모든 Storage와 Stream을 순회
3. **데이터 추출**: 각 Stream을 바이너리 형태로 저장
4. **속성 파싱**: MAPI 속성 테이블 분석
5. **결과 저장**: 구조화된 형태로 파일 시스템에 저장

## 문제 해결

### 일반적인 문제
- **"Administrator privileges are required"**: 관리자 권한으로 실행
- **"Access denied"**: 파일이 다른 프로그램에서 사용 중인지 확인
- **"File not found"**: MSG 파일 경로가 올바른지 확인

### 디버깅
- **상세 로그**: 프로그램 실행 시 모든 처리 과정이 콘솔에 출력
- **에러 메시지**: 구체적인 오류 원인과 해결 방법 제시
- **진행 상황**: 파일 처리 진행률을 실시간으로 표시

## 라이선스
이 프로젝트는 교육 및 연구 목적으로 개발되었습니다.

## 개발자
- **개발자**: real2u2l8
- **버전**: 1.0.0.1
- **설명**: Advanced MSG file analysis tool with MAPI property parsing

## 참고 자료
- [MS-OXMSG: Outlook Item (.msg) File Format](https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-oxmsg/621801cb-b617-474c-bce6-69037d73461a)
- [MAPI Property Tags](https://learn.microsoft.com/en-us/office/client-developer/outlook/mapi/mapi-property-tags)
- [Compound File Binary Format](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-cfb/53989ce4-7b05-4f8d-829d-d738d5b0131c)
