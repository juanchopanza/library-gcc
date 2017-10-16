project(library)
cmake_minimum_required(VERSION 3.7)

if (EXT_PREFIX)
    include_directories(${EXT_PREFIX}/include)
else()
    include_directories(../boost)
endif()

include_directories(./include ../ftplibpp)

add_definitions(-std=c++11)
add_definitions(-DNOSSL)

set(sources 
   include/BarcodeUtil.h
   Book.cpp
   Branch.cpp
   BranchAccess.cpp
   BranchService.cpp
   Catalog.cpp
   ClassificationService.cpp
   Customer.cpp
   Endpoint.cpp
   EndpointValidator.cpp
   FileReport.cpp
   Holding.cpp
   HoldingService.cpp
   InventoryReport.cpp
   MailDestination.cpp
   MailMessage.cpp
   Movie.cpp
   include/NameNormalizer.h
   Patron.cpp
   PatronAccess.cpp
   PatronService.cpp
   Rental.cpp
   ReportUtil.cpp
   Scanner.cpp
   ScannerState.cpp
   ScannerStateCheckin.cpp
   ScannerStateCheckout.cpp
   ScannerStateInventory.cpp
   ScannerStateWaiting.cpp
   Soundex.cpp
   StreamLineReader.cpp
   TimestampSource.cpp
   Transport.cpp
   )
add_library(library ${sources})
target_link_libraries(library pthread)
target_link_libraries(library rt)
