graph BT

&#x20;   %% Cấu hình hướng mũi tên từ dưới lên trên (Bottom-To-Top)

&#x20;   

&#x20;   %% Khai báo các node (tệp tin)

&#x20;   Main\["main.cpp"]

&#x20;   

&#x20;   subgraph Core \["Thư mục: src/Core/"]

&#x20;       Game\["CGAME.h / .cpp"]

&#x20;   end

&#x20;   

&#x20;   subgraph Entities \["Thư mục: src/Entities/"]

&#x20;       People\["CPEOPLE.h / .cpp"]

&#x20;       Vehicle\["CVEHICLE.h / .cpp"]

&#x20;       Truck\["CTRUCK.h / .cpp"]

&#x20;       Car\["CCAR.h / .cpp"]

&#x20;       Animal\["CANIMAL.h / .cpp"]

&#x20;       Bird\["CBIRD.h / .cpp"]

&#x20;       Dino\["CDINAUSOR.h / .cpp"]

&#x20;   end

&#x20;   

&#x20;   subgraph System \["Thư mục: src/System/"]

&#x20;       Config\["Config.h"]

&#x20;       RM\["ResourceManager.h"]

&#x20;   end

&#x20;   

&#x20;   SFML(("Thư viện SFML"))



&#x20;   %% Quan hệ Kế thừa (Nét đứt)

&#x20;   Truck -. "Kế thừa" .-> Vehicle

&#x20;   Car -. "Kế thừa" .-> Vehicle

&#x20;   Bird -. "Kế thừa" .-> Animal

&#x20;   Dino -. "Kế thừa" .-> Animal



&#x20;   %% Quan hệ Gọi file / Include (Nét liền)

&#x20;   Main -->|"#include"| Game

&#x20;   

&#x20;   Game -->|"#include"| People

&#x20;   Game -->|"#include"| Vehicle

&#x20;   Game -->|"#include"| Animal

&#x20;   

&#x20;   People -->|"#include"| Config

&#x20;   People -->|"#include"| RM

&#x20;   

&#x20;   Vehicle -->|"#include"| Config

&#x20;   Vehicle -->|"#include"| RM

&#x20;   

&#x20;   Animal -->|"#include"| Config

&#x20;   Animal -->|"#include"| RM

&#x20;   

&#x20;   RM -->|"#include"| SFML

&#x20;   Config -->|"#include"| SFML

