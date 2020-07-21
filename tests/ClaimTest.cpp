#include <gtest/gtest.h>
#include "jwt-cpp/jwt.h"

TEST(ClaimTest, AudienceAsString) {
	std::string token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhdWQiOiJ0ZXN0In0.WZnM3SIiSRHsbO3O7Z2bmIzTJ4EC32HRBKfLznHhrh4";
	auto decoded = jwt::decode(token);

	ASSERT_TRUE(decoded.has_algorithm());
	ASSERT_TRUE(decoded.has_type());
	ASSERT_FALSE(decoded.has_content_type());
	ASSERT_FALSE(decoded.has_key_id());
	ASSERT_FALSE(decoded.has_issuer());
	ASSERT_FALSE(decoded.has_subject());
	ASSERT_TRUE(decoded.has_audience());
	ASSERT_FALSE(decoded.has_expires_at());
	ASSERT_FALSE(decoded.has_not_before());
	ASSERT_FALSE(decoded.has_issued_at());
	ASSERT_FALSE(decoded.has_id());

	ASSERT_EQ("HS256", decoded.get_algorithm());
	ASSERT_EQ("JWT", decoded.get_type());
	auto aud = decoded.get_audience();
	ASSERT_EQ(1, aud.size());
	ASSERT_EQ("test", *aud.begin());
}

TEST(ClaimTest, SetAudienceAsString) {
	auto token = jwt::create()
		.set_type("JWT")
		.set_audience("test")
		.sign(jwt::algorithm::hs256("test"));
	ASSERT_EQ("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhdWQiOiJ0ZXN0In0.ny5Fa0vzAg7tNL95KWg_ecBNd3XP3tdAzq0SFA6diY4", token);
}

TEST(ClaimTest, AudienceAsSet) {
	std::string token = "eyJhbGciOiJub25lIiwidHlwIjoiSldUIn0.eyJhdWQiOlsidGVzdCIsInRlc3QyIl19.";
	auto decoded = jwt::decode(token);

	ASSERT_TRUE(decoded.has_algorithm());
	ASSERT_TRUE(decoded.has_type());
	ASSERT_FALSE(decoded.has_content_type());
	ASSERT_FALSE(decoded.has_key_id());
	ASSERT_FALSE(decoded.has_issuer());
	ASSERT_FALSE(decoded.has_subject());
	ASSERT_TRUE(decoded.has_audience());
	ASSERT_FALSE(decoded.has_expires_at());
	ASSERT_FALSE(decoded.has_not_before());
	ASSERT_FALSE(decoded.has_issued_at());
	ASSERT_FALSE(decoded.has_id());

	ASSERT_EQ("none", decoded.get_algorithm());
	ASSERT_EQ("JWT", decoded.get_type());
	auto aud = decoded.get_audience();
	ASSERT_EQ(2, aud.size());
	ASSERT_TRUE(aud.count("test") > 0);
	ASSERT_TRUE(aud.count("test2") > 0);
}

TEST(ClaimTest, SetAudienceAsSet) {
	auto token = jwt::create()
		.set_type("JWT")
		.set_audience({picojson::value("test"), picojson::value("test2")})
		.sign(jwt::algorithm::none{});
	ASSERT_EQ("eyJhbGciOiJub25lIiwidHlwIjoiSldUIn0.eyJhdWQiOlsidGVzdCIsInRlc3QyIl19.", token);
}

TEST(ClaimTest, SetArray) {
	std::vector<int64_t> vect = {
		100,
		20,
		10
	};
	auto token = jwt::create()
		.set_payload_claim("test", jwt::claim(vect.begin(), vect.end()))
		.sign(jwt::algorithm::none{});
	ASSERT_EQ(token, "eyJhbGciOiJub25lIn0.eyJ0ZXN0IjpbMTAwLDIwLDEwXX0.");
}

TEST(ClaimTest, SetObject) {
	std::istringstream iss{"{\"api-x\": [1]}"};
	jwt::claim object;
	iss >> object;
	ASSERT_EQ(object.get_type() , jwt::json::type::object);

	auto token = jwt::create()
		.set_payload_claim("namespace", object)
		.sign(jwt::algorithm::hs256("test"));
	ASSERT_EQ(token, "eyJhbGciOiJIUzI1NiJ9.eyJuYW1lc3BhY2UiOnsiYXBpLXgiOlsxXX19.F8I6I2RcSF98bKa0IpIz09fRZtHr1CWnWKx2za-tFQA");
}

TEST(ClaimTest, SetAlgorithm) {
	auto token = jwt::create()
		.set_algorithm("test")
		.sign(jwt::algorithm::none{});

	auto decoded_token = jwt::decode(token);
	ASSERT_EQ(decoded_token.get_algorithm(), "test");
}

TEST(ClaimTest, AsInt) {
	jwt::claim c(picojson::value((int64_t)10));
	ASSERT_EQ(c.as_int(), 10);
}

TEST(ClaimTest, AsDate) {
	jwt::claim c(picojson::value((int64_t)10));
	ASSERT_EQ(c.as_date(), std::chrono::system_clock::from_time_t(10));
}

TEST(ClaimTest, PicoJSONTraitsAccessorsThrow) {
	jwt::picojson_traits::value_type val;
	ASSERT_THROW(jwt::picojson_traits::as_array(val), std::bad_cast);
	ASSERT_THROW(jwt::picojson_traits::as_bool(val), std::bad_cast);
	ASSERT_THROW(jwt::picojson_traits::as_int(val), std::bad_cast);
	ASSERT_THROW(jwt::picojson_traits::as_number(val), std::bad_cast);
	ASSERT_THROW(jwt::picojson_traits::as_object(val), std::bad_cast);
	ASSERT_THROW(jwt::picojson_traits::as_string(val), std::bad_cast);
	ASSERT_THROW(jwt::picojson_traits::get_type(val), std::logic_error);
}

TEST(ClaimTest, PicoJSONTraitsAsBool) {
	jwt::picojson_traits::value_type val(true);
	ASSERT_EQ(jwt::picojson_traits::as_bool(val), true);
	ASSERT_EQ(jwt::picojson_traits::get_type(val), jwt::json::type::boolean);
}

TEST(ClaimTest, PicoJSONTraitsAsDouble) {
	jwt::picojson_traits::value_type val(10.0);
	ASSERT_EQ(jwt::picojson_traits::as_number(val), (int)10);
	ASSERT_EQ(jwt::picojson_traits::get_type(val), jwt::json::type::number);
}

