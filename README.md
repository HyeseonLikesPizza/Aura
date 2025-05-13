# Unreal Engine 5 - 액션 RPG 전투 시스템 포트폴리오

이 프로젝트는 언리얼 엔진 5의 Gameplay Ability System(GAS)을 기반으로 한 액션 RPG 스타일의 전투 시스템을 구현한 개인 포트폴리오입니다.  
실제 게임에 가까운 구조로 설계하며, 플레이어 스킬, 몬스터 AI, 디버프 처리, 세이브/로드 시스템, 멀티플레이 지원 등 핵심 기능들을 직접 개발했습니다.

## 주요 구현 기능

### ✅ Gameplay Ability System 기반 전투 구조

- GAS 기반 능력 구현 (Ability / Gameplay Effect / AttributeSet)
- 플레이어 레벨, 스킬 레벨, 몬스터 레벨에 따른 스탯 연동
- 화상, 스턴 등 상태 이상 처리 및 디버프 확률 발동 로직 설계
- 쿨타임, 비용, 조건 기반 발동 등 실전형 스킬 시스템

### ✅ 플레이어 스킬 구성

- **Fire Bolt**: 적중 시 일정 확률로 화상 디버프  
- **Electrocute**: 적중 시 일정 확률로 스턴 디버프  
- **Arcane Shards**, **Fire Blast**: 범위 공격 및 연계형 스킬

### ✅ 몬스터 구성 및 전투 스타일

- 근접형: Goblin, Ghoul, Demon Warrior  
- 원거리형: Goblin Slingshot, Demon Ranger  
- 마법형: Goblin Shaman (파이어볼, 소환)  
- 보스: Mutant (근접 공격, 화염 브레스, 점프 어택, Mana Reflex 등)

### ✅ 기타 시스템

- **세이브 / 로드 / 체크포인트 시스템**  
- **멀티플레이어 환경에서 플레이 가능**  
- 몬스터 처치 시 HP / MP 포션 드랍 → 회복 시스템 연결

## 개발 환경

- **Engine**: Unreal Engine 5  
- **Language**: C++  
- **System**: Gameplay Ability System (GAS), UE Multiplayer Framework

## 작업 목적

전투 콘텐츠의 구조적 설계 및 유지보수를 염두에 두고 실제 게임에 가까운 전투 시스템을 직접 구현해보는 것을 목표로 진행했습니다.  
코드뿐만 아니라 구조 설계, 밸런싱, 시스템 연동까지 고려하며 제작했습니다.

---

이 프로젝트는 개인 포트폴리오 용도로 제작되었으며 일부 기능은 개발 중입니다.