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

## Scope (Tentative pending clarification)
- Required Scope
  - Simple assembly with readily available parts
  - AC Mode
  - TV Control
  - Rate Control
  - I:E Ratio
  - PEEP Control
  - Fio2
  - disconnect alarm
  - inspiratory and expiratory pressure alarm
  - negative pressure alarm
  - Patient trigger capability
  - humidity and temperature from HME
  - exhaust filter through HME
- Stretch goals
  - Pressure data logging?
- Not POR
  - PC mode

## Proposed usage model
- Tidal volume will be set using pre-sized shims placed next to the BVM bag to limit compression to desired volume
- PEEP setting will be adjusted using a standard adjustable PEEP valve, using graphical UI pressure chart for feedback
- Peak pressure will be adjusted using the pneumatic flow rate adjusting knob and graphical UI pressure chart for feedback
- FIO2 control is done by matching the O2 flow rate to the specified TV and RR, or a venturi valve premixed supply
- I/E ratio, RR, Ppeak alarm and Pplat alarm will be set using TFT touch screen
- Detect pressure drop below PEEP to trigger patient assisted, and reset breath countdown timer
- Graphical UI will report:
  - Ppeak (peak measured pressure during inspiratory phase)
  - Pplat (final pressure at end of post-inspiratory pause)
  - PEEP (expiratory pressure when DP/DT drops below threshold)

## Prototype Phase
- Design
![v0.02](https://i.imgur.com/w6jH5BN.png)
- UI Pressure graph, with Ppeak, Pplat and PEEP detection
[![UI mockup](https://i.imgur.com/8U2wyyO.png)](https://youtu.be/rTBNxbjgaRo)
- Demonstration of pressure control ventilation
[![PC demo](https://i.imgur.com/05wxSNS.png)](https://www.youtube.com/watch?v=D5YNfeIVXSU)
- Software control flowchart
![flowchart](https://i.imgur.com/mgNiw1e.png)
