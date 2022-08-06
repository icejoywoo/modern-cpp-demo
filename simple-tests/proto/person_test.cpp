#include "gtest/gtest.h"

#include "person.pb.h"

TEST(ProtobufTest, PersonTest) {
    Person person;
    person.set_name("John");
    person.set_email("John@gmail.com");
    person.set_id(0);

    ASSERT_EQ("John", person.name());
    ASSERT_EQ("John@gmail.com", person.email());
    ASSERT_EQ(0, person.id());
}