# SpartaNum8
내일배움캠프 8번 과제

MenuLevel에서는 Start 버튼이 뜨게 만들었다. Start버튼을 누르면 등록해놓은 1단계인 Stage2가 시작된다
![image](https://github.com/user-attachments/assets/979b3752-6346-49e8-8f39-da11e9a35adf)

Stage를 시작하면 상단에 점수, 레벨 단계, 시간이 뜨고 시간은 줄어든다.
![image](https://github.com/user-attachments/assets/767c0d78-9a2e-4f33-b7d3-04d8fd6d7e26)

맵에는 아이템이 존재하는데 각 아이템에는 파티클 시스템과 사운드 시스템에 있다. 폭발 아이템의 경우 overlap되면 연기가 나고  폭발 사운드가 난다. 
![image](https://github.com/user-attachments/assets/db8e4c9a-7c97-4508-a28b-7073eeac9e53)
![image](https://github.com/user-attachments/assets/90ffe02b-2d08-4e7c-96b9-ff05ae8352f9)


![image](https://github.com/user-attachments/assets/54561217-7762-40d2-8039-33b502b573e3)

코인을 먹으면 코인 소리와 함께 왼쪽 상단에 점수가 증가한다. 우리는 게임에서 코인을 먹고 점수를 올려야 한다.
![image](https://github.com/user-attachments/assets/9f8aa181-8cd7-4e11-8b63-57877bfadb6f)

점수는 누적되며 2단계가 되면 level2가 되며 여기서도 코인을 획득해야 한다. 
![image](https://github.com/user-attachments/assets/caf26d43-59aa-4497-be8f-32deb4ad44eb)

파티클은 폭발을 해도 남아있다
![image](https://github.com/user-attachments/assets/a5df4087-d041-48c9-9ee8-a7199d67278a)


마지막인 레벨 3에 도달하고 
![image](https://github.com/user-attachments/assets/e0e63abc-c702-4fd1-988d-b3b66f96c630)

시간이 지나면 GAMEOVER과 총 점수가 적힌 화면이 띄워지며 버튼도 Restart로 바뀌게 된다. GameOver 글씨에는 사라졌다 나타났다 느낌의 효과를 주었다. restart 누르면 stage1로 돌아간다
![image](https://github.com/user-attachments/assets/ecacbc34-d227-4e8f-9156-4b315fe79827)


여기서 강의와는 다르게 구현한 점은 스폰 볼륨의 개수이다. 게임 모드에서 스폰 볼륨의 개수를 새어 총 아이템 갯수에서 볼륨 개수를 나누어 여러 스폰 볼륨에서 균등한 수의 아이템이 스폰 된다. 스테이지가 넘어갈 수록 스폰 볼륨 개수가 줄어든다.
![image](https://github.com/user-attachments/assets/bd4f8808-9fb8-448e-9cc8-c99cf2a7cdb2)
