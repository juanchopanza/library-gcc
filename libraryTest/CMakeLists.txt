project(libraryTest)
cmake_minimum_required(VERSION 3.7)

if (EXT_PREFIX)
    include_directories(${EXT_PREFIX}/include)
    link_directories(${EXT_PREFIX}/lib)
else()
    include_directories(../googletest/googletest/include ../googletest/googlemock/include)
    link_directories(../googletest/googlemock ../googletest/googlemock/gtest)
endif()

include_directories(../library/include)
link_directories(../library ../ftplibpp)

add_definitions(-std=c++11)

set(sources 
   main.cpp 
   AccountTest.cpp
   BarcodeUtilTest.cpp
   BookTest.cpp
   BranchAccessTest.cpp
   BranchServiceTest.cpp
   BranchTest.cpp
   CatalogTest.cpp
   ClassificationServiceTest.cpp
   CustomerTest.cpp
   ExamplesTest.cpp
   HoldingBarcodeTest.cpp
   HoldingServiceTest.cpp
   HoldingTest.cpp
   InventoryReportTest.cpp
   KeyedFilePersistenceTest.cpp
   KeyedMemoryPersistenceTest.cpp
   NameNormalizerTest.cpp
   PatronAccessTest.cpp
   PatronServiceTest.cpp
   PatronTest.cpp
   PersistenceTest.cpp
   ReportMailerTest.cpp
   ScannerStateCheckinTest.cpp
   ScannerStateCheckoutTest.cpp
   ScannerStateInventoryTest.cpp
   ScannerStateWaitingTest.cpp
   ScannerTestData.cpp
   StreamLineReaderTest.cpp
   TimestampSourceTest.cpp
   VectorReaderTest.cpp
   VoteTest.cpp
   )
add_executable(libraryTest ${sources})
target_link_libraries(libraryTest pthread)
target_link_libraries(libraryTest gmock)
target_link_libraries(libraryTest gtest)
target_link_libraries(libraryTest library)
target_link_libraries(libraryTest ftp++)

