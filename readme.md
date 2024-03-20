# LINE FOLLOWER TO RULE THEM ALL
Pierwsza wersja ulepa LFa autorstwa Filip Skawiński

## HOW TO

Aplikację zaimportować do MIT APP Inventor, skompilować i zainstalować na telefonie.
Ewentualnie zainstalować .apk z repo.

Po podłączeniu do AP robota albo robota do sieci WiFi kliknąć Start na górze aplikacji. Jeżeli robot działa to przycisk zmieni kolor na zielony.
Robota należy skalibrować co trwa parenaście sekund a potem już można się bawić.

## Parametry

- Kp: Wartość proporcji regulatora PID,
- Ki: Wartość całki (nie dużo daje, wartość bardzo mała np. 0.001)
- Kd: Wartość pochodnej. Należy pamiętać, że Kd > Kp. Różne są metody ale u mnie działa np 2*Kp, 7*Kp.
- BaseSpeed: szybkość skręcania robota podczas działanie algorytmu PID.
- MaxSpeed: prędkość gdy robot jedzie w prostej linii
- TurnSpeed: prędkość gdy robot wyjedzie za linie i próbuje wrócić

## Schemat podłączeń

<img alt="Schemat" src="Schemat.png">

## BOM

<table>
  <tr>
    <td>ESP32</td>
    <td><a href="https://pl.aliexpress.com/item/1005005704190069.html?spm=a2g0o.productlist.main.3.46ecNdzHNdzHkJ&algo_pvid=cf9ff183-b9f6-4b72-b5f4-1647f3390584&algo_exp_id=cf9ff183-b9f6-4b72-b5f4-1647f3390584-1&pdp_npi=4%40dis%21PLN%2115.88%2113.97%21%21%2128.22%2124.82%21%40211b612517107551505708812efff3%2112000034061352780%21sea%21PL%211653512003%21&curPageLogUid=F5yn29dzb5VY&utparam-url=scene%3Asearch%7Cquery_from%3A">Link</a></td>
  </tr>
  <tr>
    <td>Silniki N20 2000RPM 6V</td>
    <td><a href="https://pl.aliexpress.com/item/1005006557994274.html?spm=a2g0o.productlist.main.1.795cOAlvOAlvrU&algo_pvid=17ec320a-65ef-4670-a0b2-bc2f20f15879&algo_exp_id=17ec320a-65ef-4670-a0b2-bc2f20f15879-0&pdp_npi=4%40dis%21PLN%2113.74%2114.02%21%21%213.39%213.46%21%40211b612517107551947071927efff3%2112000037663434199%21sea%21PL%211653512003%21&curPageLogUid=QBfC6nN1PA1C&utparam-url=scene%3Asearch%7Cquery_from%3A">Link</a></td>
  </tr>
  <tr>
    <td>Listwa czujników IR</td>
    <td><a href="https://pl.aliexpress.com/item/1005003333788694.html?spm=a2g0o.order_list.order_list_main.132.51f21c24Y7fKGh&gatewayAdapt=glo2pol">Link</a></td>
  </tr>
  <tr>
    <td>Sterownik silników</td>
    <td><a href="https://pl.aliexpress.com/item/1005003401989364.html?spm=a2g0o.order_list.order_list_main.137.51f21c24Y7fKGh&gatewayAdapt=glo2pol">Link</a></td>
  </tr>
  <tr>
    <td>Regulator napięcia</td>
    <td><a href="https://pl.aliexpress.com/item/4000016739581.html?spm=a2g0o.detail.pcDetailTopMoreOtherSeller.2.3429TGExTGExLT&gps-id=pcDetailTopMoreOtherSeller&scm=1007.40050.354490.0&scm_id=1007.40050.354490.0&scm-url=1007.40050.354490.0&pvid=bae215d9-c8e1-49fe-b84e-9d412304ed59&_t=gps-id:pcDetailTopMoreOtherSeller,scm-url:1007.40050.354490.0,pvid:bae215d9-c8e1-49fe-b84e-9d412304ed59,tpp_buckets:668%232846%238110%231995&pdp_npi=4%40dis%21PLN%215.15%214.74%21%21%211.27%211.17%21%40211b61a417107524588842017ee689%2112000034624191356%21rec%21PL%211653512003%21&utparam-url=scene%3ApcDetailTopMoreOtherSeller%7Cquery_from%3A">Link</a></td>
  </tr>
  <tr>
    <td>Płytka prototypowa</td>
    <td><a href="https://pl.aliexpress.com/item/1005006100148769.html?spm=a2g0o.productlist.main.2.631cdsmgdsmgFx&algo_pvid=899cb1f6-c89a-499a-8537-0dbaeac1d593&algo_exp_id=899cb1f6-c89a-499a-8537-0dbaeac1d593-1&pdp_npi=4%40dis%21PLN%213.20%213.01%21%21%215.69%215.34%21%40211b612517107553350064239efff3%2112000035755220383%21sea%21PL%211653512003%21&curPageLogUid=7hSxv22ADHy3&utparam-url=scene%3Asearch%7Cquery_from%3A">Link</a></td>
  </tr>
  <tr>
    <td>LiPo 2S 7.4V</td>
    <td>Brak linku</td>
  </tr>
  <tr>
    <td>Headery i sockety</td>
    <td>Brak linku</td>
  </tr>
  <tr>
    <td>Kabelki</td>
    <td>Brak linku</td>
  </tr>
  <tr>
    <td>Koła</td>
    <td>Brak linku</td>
  </tr>
</table>


## Rady Norberta

Zrobić czujniki w konfiguracji 1,3,4,5,6,8

## TO DO

- komentarze
