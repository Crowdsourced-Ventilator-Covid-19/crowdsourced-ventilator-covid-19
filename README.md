# crowdsourced-ventilator-covid-19
## Problem Statement
How to create a safe ventilator that can be used as a last resort where a physician has no choice but to use it or lose a life.  Obviously it will not be FDA approved.  In fact, a wonderful outcome would be if somehow US industry could step up to solve the problem of the ventilator shortage.  However, based on the data we are seeing from Johns Hopkins site above it is likely by the time industry succeeds thousand of people may be in respiratory distress,  particularly in New York City and other big hot spots.  So far no deaths in MA but statistically speaking this will not last for long.

Can we make a difference?
> Honestly I am not sure we can,  but we can try to help, even if we save one life long enough to get on a commercially available ventilator.  In my religion, it is said that saving one life saves an entire world.
-- Joe Siegel

So can we design a ventilator good enough to help and do no harm to the patient?   Can this design be simple enough to put together from components readily available on Amazon or hobby shops or any place that can source the components.  Can we crowd source the production of this design and the cost to build it.  If it can be simple, crowd sourced and done quickly we might be able to help.  The other option is to take the design to someone who will put them into production until the Medical Industrial Complex takes over.

## References
- [Johns Hopkins dashboard](https://gisanddata.maps.arcgis.com/apps/opsdashboard/index.html#/bda7594740fd40299423467b48e9ecf6)
- [Live Interactive dashboard](https://datastudio.google.com/embed/reporting/f56febd8-5c42-4191-bcea-87a3396f4508/page/k1XIB)
- [Epidemic Calculator](http://gabgoh.github.io/COVID/index.html)
- [JHU Time Series Dataset](https://github.com/CSSEGISandData/COVID-19/tree/master/csse_covid_19_data)
- [Mechanical Ventilation video series](https://www.youtube.com/watch?v=i6hmGVBbIJk)
- [MIT design](https://www.fastcompany.com/1661942/mit-students-invent-100-ventilator-for-disaster-zones)
- [Hackaday effort](https://hackaday.com/2020/03/12/ultimate-medical-hackathon-how-fast-can-we-design-and-deploy-an-open-source-ventilator/)
- [Open Source Medical Supplies Facebook Group](https://www.facebook.com/groups/670932227050506/post_tags/?post_tag_id=674736673336728)

## Questions for Dr. Kagle
- Which modes are necessary, and in what priority?
- Compact portable like MIT design? or Nasal cannula-based NIV?
- Can the hospital provide a pre-mixed FIO2 external air supply?
- What controls are necessary, and in what priority?
- What instrumentation is necessary, and in what priority?
- Which alarms are necessary?

## Scope (Tentative pending clarification)
- Required Scope
  - Simple assembly with readily available parts
  - AC Mode
    - Patient trigger capability
    - Backup rate control
    - TV control
    - PEEP control
    - Humidifier
  - Exhaust capture?
  - Peak and plateau pressure alarms
  - Pressure timeseries logs
- Stretch goals
  - PC Mode?
  - Air pressure source?
  - FIO2 control?
  - Temperature control?
  - Flow control? (Inhalation/Exhalation duty cycle)
- Not POR

## Prototype Phase
- Demonstrate functionality of necessary modes, controls, alarms and instrumentation
- Test reliability
- Refine design

