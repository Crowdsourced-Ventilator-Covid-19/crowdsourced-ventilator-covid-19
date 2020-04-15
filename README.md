# crowdsourced-ventilator-covid-19
## Problem Statement
How to create a safe ventilator that can be used as a last resort where a physician has no choice but to use it or lose a life.  Obviously it will not be FDA approved.  In fact, a wonderful outcome would be if somehow US industry could step up to solve the problem of the ventilator shortage.  However, based on the data we are seeing from Johns Hopkins site above it is likely by the time industry succeeds thousand of people may be in respiratory distress,  particularly in New York City and other big hot spots.  So far no deaths in MA but statistically speaking this will not last for long.

Can we make a difference?
> Honestly I am not sure we can,  but we can try to help, even if we save one life long enough to get on a commercially available ventilator.  In my religion, it is said that saving one life saves an entire world.
-- Joe Siegel

So can we design a ventilator good enough to help and do no harm to the patient?   Can this design be simple enough to put together from components readily available on Amazon or hobby shops or any place that can source the components.  Can we crowd source the production of this design and the cost to build it.  If it can be simple, crowd sourced and done quickly we might be able to help.  The other option is to take the design to someone who will put them into production until the Medical Industrial Complex takes over.

## Team
1. Joe Siegel - Electrical/Physics Engineer  Advanced Micro Devices
1. Steve Bunes - Engineer/Physics and CEO Rise Time Inc. Chicago
1. Chuck Tung - Electrical Engineer / Mechanical Engineer Advanced Micro Devices
1. Dr. David Kagle - Anesthesiologist Emerson Hospital
1. Kenny House - Biomedical Engineer and currently works for AMD 


## Resources
### Specs
- [UK Rapidly Manufactured Ventilator Spec](https://www.gov.uk/government/publications/coronavirus-covid-19-ventilator-supply-specification/rapidly-manufactured-ventilator-system-specification)
- [Pandemic Ventilator User Requirements](https://docs.google.com/spreadsheets/u/2/d/1XAo2LKQBSAnemD73kKWN7G7QsSLVRSNY9SQNuliBCzY/edit#gid=0)
- [OSCMS Requirements](https://docs.google.com/document/d/15kqUPPI7bYL6dnCetOeDSyE8IG5pHVmtg8Ju4yzGlF8/edit?fbclid=IwAR31p6i7iZhFuL7ejLUs5mz9gmRXWi1kWoOG2wR_Lyqr3UgzaOcz6t2VNlk#heading=h.1pr0nkc86xgg)
- [Ventilator spec](https://drive.google.com/open?id=1mXAS4-lcwsU2nRVF3crRuBIVWJfbnB5P)
### Other Projects
- [OpenSourceCoronavirusProjects](https://github.com/cyborg527/OpenSourceCoronavirusProjects)
- [OpenSourceVentilator](https://opensourceventilator.ie/)
- [MIT design](https://www.fastcompany.com/1661942/mit-students-invent-100-ventilator-for-disaster-zones)
- [Hackaday effort](https://hackaday.com/2020/03/12/ultimate-medical-hackathon-how-fast-can-we-design-and-deploy-an-open-source-ventilator/)
### Dashboards
- [Johns Hopkins dashboard](https://gisanddata.maps.arcgis.com/apps/opsdashboard/index.html#/bda7594740fd40299423467b48e9ecf6)
- [Live Interactive dashboard](https://datastudio.google.com/embed/reporting/f56febd8-5c42-4191-bcea-87a3396f4508/page/k1XIB)
- [Epidemic Calculator](http://gabgoh.github.io/COVID/index.html)
- [JHU Time Series Dataset](https://github.com/CSSEGISandData/COVID-19/tree/master/csse_covid_19_data)
### Other
- [Mechanical Ventilation video series](https://www.youtube.com/watch?v=i6hmGVBbIJk)
- [Open Source Medical Supplies Facebook Group](https://www.facebook.com/groups/670932227050506/post_tags/?post_tag_id=674736673336728)
- [A low oxygen consumption pneumatic ventilator for emergency construction during a respiratory failure pandemic - Williams](https://l.facebook.com/l.php?u=https%3A%2F%2Flookaside.fbsbx.com%2Ffile%2FWilliams_et_al-2010-Anaesthesia.pdf%3Ftoken%3DAWx69kPx0nI5HEpVeaNy24bn4q1GDzC0sXTHRSZhE5KIxPyQ67eEu4S7N0iHn_JngGeEK6sNljjXqsA0bA2tAop5FZhIAJotbVWZG8MHPUSA61l7TgDelxZyH2fv6vmKNmL1gULxPK_-b8P3h3ojDA-C_5E6aT0WHfSQDe-etnz0yQ&h=AT3qvgoCUalOjiZATD5rFSJ50BaXUBusOQzW24deVutMAoYmEEW2gb7SYGIeyHJHv5q-TYAeBxpWslhvpChvipB0kV3w04YnqNlw6mtW9Ulel8722b-9MIDKZM1h07lpOwy1N8V3uD4&__tn__=H-R&c[0]=AT2hPfdfSJtkRwnPiZanBKqyTAIH8YzXrv1SWUdh0CvUafrDPVMH3qYw4MAtS64r40eAcNFiGvjK22xvQ87VvWOnltyAbapza0FJKsrX69uNKpJWSq5YWYZUeS-ccZNzSWObEsIWBGhfaV-nHj7KMA)
- [snubber design](https://www.cde.com/resources/technical-papers/design.pdf)

## Scope (Tentative pending clarification)
- Required Scope
  - Simple assembly with readily available parts
  - AC Mode in both VCV and PCV
  - TV Control
  - Rate Control
  - I:E Ratio
  - PEEP Control
  - Fio2
  - disconnect alarm
  - inspiratory and expiratory pressure alarm
  - Patient trigger capability
  - humidity and temperature from HME
  - exhaust filter through HME
  - Flow and volume sensing
- Stretch goals
  - Pressure data logging?
- Not POR
  - PC mode

## Proposed usage model
- TV, I/E ratio, RR, Ppeak alarm and Pplat alarm will be set using TFT touch screen
- PEEP setting will be adjusted using a standard adjustable PEEP valve, using graphical UI pressure chart for feedback
- Peak pressure will be adjusted using the compressed air regulator and graphical UI pressure chart for feedback
- Max flow rate and Tp is adusted with the adjustable speed exhaust filter
- FIO2 control is done by matching the O2 flow rate to the specified TV and RR, or a venturi valve premixed supply
- Detect pressure drop below PEEP to trigger patient assisted, and reset breath countdown timer
- Graphical UI will report:
  - Ppeak (peak measured pressure during inspiratory phase)
  - Pplat (final pressure at end of post-inspiratory pause)
  - PEEP (expiratory pressure when DP/DT drops below threshold)

## BOM

| Item | Price |
| --- | --- |
| Arduino Mega 2560 R3 | $17 |
| [4ch 5VDC Relay with Optocoupler](https://www.amazon.com/SainSmart-101-70-101-4-Channel-Relay-Module/dp/B0057OC5O8/ref=sr_1_2?dchild=1&keywords=4ch+5v+dc+relay&qid=1586456845&s=industrial&sr=1-2) | $10 |
| 2x [I2C differential breakout](https://www.sparkfun.com/products/14589) | 2x $11 |
| 2x [MPRLS](https://www.adafruit.com/product/3965) | 2x $15 |
| [TFT Touchscreen](https://www.amazon.com/gp/product/B07NWH47PV/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) | $17 |
| [5way 2pos solenoid](https://www.amazon.com/Baomain-Pneumatic-Solenoid-Internally-Electrical/dp/B01J3CE3EE/ref=sr_1_4?crid=1K7DOXGGJAF8T&dchild=1&keywords=5+way+2+position+pneumatic+valve&qid=1586062215&sprefix=5-way+2+pos%2Caps%2C159&sr=8-4) | $12 |
| [20mm Bore 100mm stroke pneumatic cylinder](https://www.amazon.com/Sydien-Pneumatic-Cylinder-Connector-MAL20X100/dp/B07J9V84VH/ref=sr_1_2?dchild=1&keywords=pneumatic+cylinder+100mm&qid=1586062300&sr=8-2) | $14 |
| [6mm to 1/4" NPT push connects](https://www.amazon.com/TOUHIA-Pneumatic-Straight-Connect-Fittings/dp/B07RXVQPH6/ref=sr_1_4?crid=2NKU48K7C26S9&dchild=1&keywords=1%2F4+npt+push+to+connect+fittings&qid=1586062414&sprefix=1%2F4+npt+pu%2Caps%2C154&sr=8-4) | $8 |
| [Pressure regulator](https://www.amazon.com/Primefit-CR1401G-Replacement-Regulator-steel-Protected/dp/B00IDCYIJ6/ref=sr_1_2?dchild=1&keywords=pressure+regulator&qid=1586062545&sr=8-2) | $6 |
| [Adjustable silencers](https://www.amazon.com/uxcell-Adjustable-Pneumatic-Exhaust-Silencer/dp/B07K39R9Y3/ref=sr_1_2?dchild=1&keywords=pneumatic+adjustable+silencer+1%2F4+npt&qid=1586062735&sr=8-2) | $8 |
| [6mm tubing](https://www.amazon.com/Uxcell-a13080200ux0301-Blue-29-5Ft-Polyurethane/dp/B00G9JQ2JY/ref=sr_1_5?crid=366JZUYPHKWGG&dchild=1&keywords=6mm+pneumatic+tubing&qid=1586062852&sprefix=6mm+pnu%2Caps%2C154&sr=8-5)  | $8 |
| [Project Box](https://www.amazon.com/Hammond-1591USBK-Plastic-Project-Inches/dp/B007POB8PK/ref=sr_1_15?dchild=1&keywords=electronic+project+box&qid=1586062900&sr=8-15) | $9 |
| [Teflon tape](https://www.amazon.com/Dixon-Valve-TTB75-Industrial-Temperature/dp/B003D7K8E0/ref=sr_1_8?dchild=1&keywords=teflon+tape&qid=1586097443&sr=8-8) | $2 |
| [Differential Pressure Sensor](https://ncd.io/differential-pressure-sensor-arduino/) | $60 |
| [Check Valve](https://www.wnlproducts.com/product/replacement-valves-for-cpr-mask/) | $3 |
| [1" NPT 12VDC Solenoid](https://www.amazon.com/gp/product/B018WRJYSG/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1) | $26 |
| [1/8" ID tubing](https://www.amazon.com/gp/product/B0861X9BJT/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1) | $9 |
| [2.5mm ID tubing](https://www.amazon.com/gp/product/B010V5GMEG/ref=ppx_yo_dt_b_asin_title_o04_s01?ie=UTF8&psc=1) | $6 |
| [Tower light and alarm](https://www.adafruit.com/product/2994) | $25 |
| [2x 1N4004 diode](https://www.sparkfun.com/products/14884)| $1 |
| [Venturi flow meter tube](https://a360.co/2JGjDKU) | $10 |
| [2x PEEP valve](https://www.amazon.com/gp/product/B00Q0SBGR2/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) | $24 |
| Total | ~$300 |

## Prototype Phase
- Design

![v0.03](https://i.imgur.com/NC8aWRx.png)

- Demonstration
[![UI demo](https://i.imgur.com/CcICFsl.png)](https://www.youtube.com/watch?v=sNcdRJmJqwU)

- Software control flowchart

![flowchart](https://i.imgur.com/Xz6PEpd.png)
