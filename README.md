# MazeGame
### 미로를 외워서 탈출하는 C++ 콘솔 게임
머릿속의 지도와 감각으로 미로를 탈출하는 텍스트 형식의 콘솔 게임

### 🚀개발 동기
* 부트캠프 과제를 창의성 있고 완벽하게 수행하고 싶은 열망
  * 제주도 메이즈랜드의 경험에서 영감을 받음
* C++ 활용 역량 강화
* 알고리즘을 적용한 프로젝트 개발
<br></br>

### 📜게임 설명
* 설정한 크기에 맞는 미로 랜덤 생성
* 생성된 미로를 확인하고 게임 시작
* 텍스트 형식으로 미로 진행 및 탈출
<br></br>

### 🎯주요 게임 기능
설명|미로 생성
:-:|:-:
이미지|<img src="https://github.com/developerYHLee/MazeProject/assets/82407061/7cd5bf68-6e59-40f7-b151-8b2760e4ff93"/>
기능|입력한 크기의 미로 랜덤 생성
활용 알고리즘|SideWinder 미로 생성

<br></br>

설명|최단 거리
:-:|:-:
이미지|<img src="https://github.com/developerYHLee/MazeProject/assets/82407061/a24fe174-bd39-40e0-ba8b-727b1c5ca17f"/>
기능|입구부터 출구까지의 최단 거리
활용 알고리즘|BFS

최단 거리를 활용한 기능
* 랜덤으로 출구 배치

<br></br>

설명|최단 경로
:-:|:-:
이미지|<img src="https://github.com/developerYHLee/MazeProject/assets/82407061/0f14ed3a-06a0-47c7-abc5-9fe9da35bfdc"/>
기능|입구부터 출구까지의 최단 경로
활용 알고리즘|Backtracking

최단 경로를 활용한 기능
* 게임 중간에 현재 위치에서 출구까지의 경로 확인
* AI 탈출

<br></br>

### 🎞영상
#### 게임 시작 전
https://github.com/developerYHLee/MazeProject/assets/82407061/e1ab3d5a-5886-4a79-82af-d355b2e77d81

<br></br>

#### AI 탈출
https://github.com/developerYHLee/MazeProject/assets/82407061/c2cc6fb9-3630-4794-b22f-06d51d6f2559

<br></br>

#### 전체 영상
https://github.com/developerYHLee/MazeProject/assets/82407061/4eb23bb4-e1eb-4515-a121-67970bd808e3

<br></br>

#### 미로 크게 생성
https://github.com/developerYHLee/MazeProject/assets/82407061/c8934bf3-c06d-4068-a1b1-c2406d8551c3

<br></br>



### 🔧노력한 부분
* 기능별로 개발하도록 노력(객체화)
  *  효율적인 재사용성과 유지보수
    <br></br>
  *  예시1) 게임 도중 잘못된 최단 경로를 출력(유지보수)
     ![경로 찾기 오류](https://github.com/developerYHLee/MazeProject/assets/82407061/051a5ccb-ddcc-4a03-9253-6c75517d352e)
     *  최단 경로를 찾는 코드를 디버깅해서 어떤 문제인지 내부적인 오류 파악
     *  최단 경로 기능의 코드만 변경해서 문제 해결
     *  빠른 유지보수로 문제 해결
    <br></br>
  *  예시2) 최단 거리 및 최단 경로 기능(재사용)
   1. 출구 랜덤 배치
      *  미로 생성 시 입구와 출구까지의 최단 거리가 가까우면 출구 재배치
        <br></br>
   2. 게임 중 현재 위치에서 출구까지 경로 출력
      *  최단 경로를 출구 대신 현재 위치부터 탐색
        <br></br>
   3. AI 탈출
      *  최단 경로를 탐색하고 경로 저장 후 저장된 경로를 활용하여 AI 이동
        <br></br>  
    *  세 가지 모두 이미 구현한 기능을 재사용해서 간단하게 새로운 기능 구현

<br></br>
* 보완할 점
   * Board 클래스의 기능 사용 부분이나 StartGame 클래스의 Input 받는 기능을 따로 클래스로 분리 필요(추상화 부족)
   * 더 복잡한 미로로 변경(Prim, Eller's 알고리즘)
   * 다채로운 기능 추가 필요(설명서, 퍼즐 요소 추가, 움직이는 문 등)

<br></br>
  
### 🛠사용한 기술
**언어** : <img src="https://img.shields.io/badge/C++-purple?style=flat-square&logo=cplusplus&logoColor=#00599C"/>

### 🏆배운 점
* 객체화를 통한 재사용성 및 유지보수 역량 향상
* 알고리즘을 프로젝트에 적용하는 능력 향상
* C++ 활용 역량 향상

