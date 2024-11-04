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
    <td>ESP32-S3</td>
    <td><a href="https://pl.aliexpress.com/item/1005007656791597.html?spm=a2g0o.productlist.main.13.545ee3906jK00Z&algo_pvid=ab0496bf-58f1-44b3-bbdb-f1086f42cc5c&algo_exp_id=ab0496bf-58f1-44b3-bbdb-f1086f42cc5c-6&pdp_npi=4%40dis%21PLN%2129.99%2129.99%21%21%217.29%217.29%21%40211b807017303099795513300ef8ed%2112000041678227215%21sea%21PL%211653512003%21X&curPageLogUid=VWNVc5cLpqUa&utparam-url=scene%3Asearch%7Cquery_from%3A">Link</a></td>
  </tr>
  <tr>
    <td>Silniki N20 1000-2000RPM 6V</td>
    <td><a href="https://pl.aliexpress.com/item/1005005269066699.html?spm=a2g0o.productlist.main.27.cc4b657amqK5Dy&algo_pvid=95db3c5d-38b6-40a0-9bf5-c7dee7782784&algo_exp_id=95db3c5d-38b6-40a0-9bf5-c7dee7782784-13&pdp_npi=4%40dis%21PLN%2115.70%2115.70%21%21%2127.26%2127.26%21%40210390b817303109208393923e5e9f%2112000032429643050%21sea%21PL%211653512003%21X&curPageLogUid=oLB5q8NQthUe&utparam-url=scene%3Asearch%7Cquery_from%3A">Link</a></td>
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
    <td><a href="https://pl.aliexpress.com/item/1005006245122273.html?spm=a2g0o.detail.pcDetailTopMoreOtherSeller.2.70efpUw8pUw8u5&gps-id=pcDetailTopMoreOtherSeller&scm=1007.40050.354490.0&scm_id=1007.40050.354490.0&scm-url=1007.40050.354490.0&pvid=badf5927-ea6f-4215-a7d9-8c04e6ace2fc&_t=gps-id:pcDetailTopMoreOtherSeller,scm-url:1007.40050.354490.0,pvid:badf5927-ea6f-4215-a7d9-8c04e6ace2fc,tpp_buckets:668%232846%238110%231995&pdp_npi=4%40dis%21PLN%213.71%213.71%21%21%216.43%216.44%21%40211b819117307263097706984e0372%2112000036448444700%21rec%21PL%211653512003%21XZ&utparam-url=scene%3ApcDetailTopMoreOtherSeller%7Cquery_from%3A">Link</a></td>
  </tr>
  <tr>
    <td>Płytka prototypowa</td>
    <td><a href="https://pl.aliexpress.com/item/1005006100148769.html?spm=a2g0o.productlist.main.2.631cdsmgdsmgFx&algo_pvid=899cb1f6-c89a-499a-8537-0dbaeac1d593&algo_exp_id=899cb1f6-c89a-499a-8537-0dbaeac1d593-1&pdp_npi=4%40dis%21PLN%213.20%213.01%21%21%215.69%215.34%21%40211b612517107553350064239efff3%2112000035755220383%21sea%21PL%211653512003%21&curPageLogUid=7hSxv22ADHy3&utparam-url=scene%3Asearch%7Cquery_from%3A">Link</a></td>
  </tr>
  <tr>
    <td>LiPo 2S 7.4V</td>
    <td><a href="https://botland.com.pl/akumulatory-li-pol-2s-74v-/7876-pakiet-li-pol-dualsky-300mah-30c-2s-74v-6941047107403.html">Link</a></td>
  </tr>
  <tr>
    <td>Headery i sockety</td>
    <td><a href="https://pl.aliexpress.com/item/4000873858801.html?spm=a2g0o.detail.pcDetailTopMoreOtherSeller.2.6ae810Xb10XbcI&gps-id=pcDetailTopMoreOtherSeller&scm=1007.40050.354490.0&scm_id=1007.40050.354490.0&scm-url=1007.40050.354490.0&pvid=1e43e31e-8b77-441c-8f33-47458f6eafbe&_t=gps-id:pcDetailTopMoreOtherSeller,scm-url:1007.40050.354490.0,pvid:1e43e31e-8b77-441c-8f33-47458f6eafbe,tpp_buckets:668%232846%238110%231995&pdp_npi=4%40dis%21PLN%218.72%218.72%21%21%212.12%212.12%21%40211b6c1717303109843884282e2dfb%2110000010058190554%21rec%21PL%211653512003%21X&utparam-url=scene%3ApcDetailTopMoreOtherSeller%7Cquery_from%3A">Link</a></td>
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

