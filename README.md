# NumberBaseball - 멀티플레이어 숫자야구 게임

Unreal Engine 5.5 기반의 멀티플레이어 숫자야구 게임입니다.
2명의 플레이어가 3D 월드 내 위젯을 통해 서로 숫자를 맞추는 턴제 게임입니다.

## 게임 규칙

* 서버가 1~9 사이 중복 없는 3자리 비밀번호를 생성합니다.
* 각 플레이어는 3회의 기회 안에 상대의 비밀번호를 맞춰야 합니다.
* 판정 기준:
  * **Strike(S)**: 숫자와 자리가 모두 일치
  * **Ball(B)**: 숫자는 존재하지만 자리가 불일치
  * **OUT**: 일치하는 숫자가 하나도 없음
* 3S0B를 먼저 달성한 플레이어가 승리합니다.
* 모든 플레이어의 기회가 소진되면 무승부입니다.

## 게임 흐름

```
Waiting → WriteName → Playing → GameOver
```

1. **Waiting**: 두 플레이어가 3D 위젯에 접근하여 상호작용 키로 위젯을 엽니다.
2. **WriteName**: 각 플레이어가 자신의 이름을 입력합니다.
3. **Playing**: 턴제로 진행되며, 10초 제한시간 내에 3자리 숫자를 입력합니다.
4. **GameOver**: 승리 또는 무승부 결과가 표시되고 입력이 차단됩니다.

## 프로젝트 구조

```
Source/ChatSystem/
├── System/
│   ├── MainGameModeBase.h/.cpp      # 비밀번호 생성, 판정, 턴 관리, 타이머
│   └── MainGameStateBase.h/.cpp     # GamePhase 복제 및 상태 전이
├── Player/
│   ├── MainPlayerController.h/.cpp  # Server/Client RPC, 채팅 메시지 처리
│   └── MainPlayerState.h/.cpp       # 이름, 위젯 열림 여부, 시도 횟수
├── UI/
│   └── NumberBaseballWidget.h/.cpp   # 채팅 UI, 타이머, 기회 표시, 턴 표시
├── Interaction/
│   └── NumberBaseballInteraction.h/.cpp  # 3D 위젯 오버랩 및 상호작용
├── ChatSystemCharacter.h/.cpp       # 캐릭터, WidgetInteraction 컴포넌트
└── ChatSystemGameMode.h/.cpp        # 기본 GameMode
```

## 네트워킹 아키텍처

### Server RPC (클라이언트 → 서버)

| RPC | 역할 |
|-----|------|
| `ServerRPCSetWidgetOpen` | 위젯 열림/닫힘 상태 전달 |
| `ServerRPCSetPlayerName` | 플레이어 이름 설정 |
| `ServerRPCChatMessage` | 추측 숫자 전송 및 판정 요청 |

### Client RPC (서버 → 클라이언트)

| RPC | 역할 |
|-----|------|
| `ClientRPCReceiveChatMessage` | 판정 결과 및 시스템 메시지 수신 |
| `ClientRPCSetTurn` | 턴 전환 알림 |
| `ClientRPCConsumeAttempt` | 남은 기회 갱신 |

### Property Replication

| 클래스 | 프로퍼티 | 설명 |
|--------|----------|------|
| GameState | `GamePhase` | OnRep 콜백으로 UI 자동 갱신 |
| PlayerState | `bIsNameSet` | 이름 설정 완료 여부 |
| PlayerState | `bIsWidgetOpen` | 위젯 열림 여부 |

## 구현 기능

### 필수 기능

* 1~9 사이 중복 없는 3자리 비밀번호 생성
* 입력값 유효성 검사 (3자리 여부, 문자 포함 여부, 중복 여부)
* Strike/Ball/OUT 판정 로직
* 각 플레이어 3회 기회 부여 및 차감
* 남은 기회 이미지 UI 표시
* 3S0B 승리 판정 및 기회 소진 시 무승부 판정
* 게임 결과 채팅 브로드캐스트
* GameOver 시 입력 차단

### 도전 기능

* 서버 기반 10초 턴 타이머 및 클라이언트 실시간 카운트다운 UI
* 시간 초과 시 자동 기회 차감 및 "시간 초과!" 메시지 브로드캐스트
* SwitchTurn 턴 전환 및 비턴 시 입력창 비활성화

### 나만의 기능

* 숫자야구 UI를 3D 공간의 WidgetComponent로 구현
* 오버랩 트리거 영역에서 상호작용 키로 위젯 토글
* WidgetInteractionComponent를 카메라에 부착하여 마우스 클릭으로 3D 위젯 조작
* PlayerState 기반 위젯 열림 상태 추적 및 단계적 GamePhase 전이 시스템

## 개발 환경

* **엔진**: Unreal Engine 5.5
* **언어**: C++
* **클라이언트**: 2인 멀티플레이
