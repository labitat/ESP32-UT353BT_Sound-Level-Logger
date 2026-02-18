

/*
AD packet raw: 0x (02 01 06) (08 09 55543335334254) (03 03 12FF) (14 FF AABB 10053B202033362E346442413D34000418)
                                                                                   202033362E34644241 = "  36.4dBA"


0x020106080955543335334254030312FF14FFAABB10053B202033362E346442413D34000418
  ||                                              ||        ||  ||  ||||        
  01                                              25        30  32  || L 00=NORM 04=MIN 08=MAX
                                                   _ 3 6 . 4 d B A   L 33=SLOW 34=FAST



BleConnect: UT353BT
Service: FF12
Characteristic: FF01
MAX: AABB04303F01D8
MIN: AABB04304001D9
HOLD: AABB04304201DB


 
Generic Attribute
UUID: 0x1801
PRIMARY SERVICE
  Service Changed
  UUID: 0x2A05  <--- !!!_ACTIVATE_!!!
  Properties: INDICATE
  Descriptors:
    Client Characteristic Configuration
    UUID: 0x2902
    Value: "Notifications and indications disabled"/"Indications enabled"

Generic Access
UUID: 0x1800
PRIMARY SERVICE
  Device Name
  UUID: 0x2A00
  Properties: READ
  Value: UT353BT

Unknown Service
UUID: 0xFF12
PRIMARY SERVICE
  Characteristic:
    UUID: 0xFF02  <--- !!!_ACTIVATE_!!!
    Properties: NOTIFY
    Descriptors:
      Client Characteristic Configuration
        UUID: 0x2902
        Value: "Notifications and indications disabled"/"Notifications enabled"
      Characteristic User Description
        UUID: 0x2901
        Value: "Data Out"
  Characteristic:
    UUID: 0xFF06
    Properties: READ, WRITE
    Value: (0x) 55-54-33-35-33-42-54, "UT353BT"
    Descriptors:
      Characteristic User Description
        UUID: 0x2901
        Value: BT Name
  Characteristic:
    UUID: 0xFF08
    Properties: READ, WRITE
    Value: (0x) 40-01
    Descriptors:
      Characteristic User Description
        UUID: 0x2901
        Value: Adv. Interval
  Characteristic:
    UUID: 0xFF0B
    Properties: READ, WRITE
    Value: (0x) 00
    Descriptors:
      Characteristic User Description
        UUID: 0x2901
        Value: Mode Control

Unknown Service
UUID: 0000d0ff-3c17-d293-8e48-14fe2e4da212
PRIMARY SERVICE
  Characteristic:
    UUID: 0x
    Properties: 
    Value: 
    Descriptors:
      Client Characteristic Configuration
        UUID: 0x
        Value: 
      Characteristic User Description
        UUID: 0x
        Value: 
  Characteristic:
    UUID: 0x
    Properties: 
    Value: 
    Descriptors:
      Client Characteristic Configuration
        UUID: 0x
        Value: 
      Characteristic User Description
        UUID: 0x
        Value: 
  Characteristic:
    UUID: 0x
    Properties: 
    Value: 
    Descriptors:
      Client Characteristic Configuration
        UUID: 0x
        Value: 
      Characteristic User Description
        UUID: 0x
        Value: 


*/