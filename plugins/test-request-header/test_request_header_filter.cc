#include <string>
#include <stdexcept>
#include <unistd.h>
#include "test_request_header_filter.h"

#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Http {

TestRequestHeaderFilterConfig::TestRequestHeaderFilterConfig(
    const sample::TestRequestEcho& proto_config)
    : key_(proto_config.key()), val_(proto_config.val()) {}

TestRequestHeaderFilter::TestRequestHeaderFilter(TestRequestHeaderFilterConfigSharedPtr config)
    : config_(config) {}

TestRequestHeaderFilter::~TestRequestHeaderFilter() {}

void TestRequestHeaderFilter::onDestroy() {}

const LowerCaseString TestRequestHeaderFilter::headerKey() const {
  return LowerCaseString(config_->key());
}

const std::string TestRequestHeaderFilter::headerValue() const {
  return config_->val();
}

const std::string gen_random(const int len) {
  static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return tmp_s;
}

void gen_random_ref(const int len, std::string& tmp_s) {
  static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
}
FilterHeadersStatus TestRequestHeaderFilter::decodeHeaders(RequestHeaderMap& headers, bool) {
  // add a header
  for(int i=0; i < 90; i++) {
    headers.setCopy(LowerCaseString(std::string("user-agent-") + std::to_string(i)), std::string("test-sample-header-response-benchmark") + std::to_string(i));
  }
  std::string tmp_s1, tmp_s2, tmp_s3;
  tmp_s1.reserve(1004);
  tmp_s2.reserve(1004);
  tmp_s3.reserve(1004);

  tmp_s1 += std::string("XVlBzgbaiCMRAjWwhTHctcuAxhxKQFDaFpLSjFbcXoEFfRsWxPLDnJObCsNVlgTeMaPEZQleQYhYzRyWJjPjzpfRFEgmotaFetHsbZRjxAwnwekrBEmfdzdcEkXBAkjQZLCtTMtTCoaNatyyiNKAReKJyiXJrscctNswYNsGRussVmaozFZBsbOJiFQGZsnwTKSmVoiGLOpbUOpEdKupdOMeRVjaRzLNTXYeUCWKsXbGyRAOmBTvKSJfjzaLbtZsyMGeuDtRzQMDQiYCOhgHOvgSeycJPJHYNufNjJhhjUVRuSqfgqVMkPYVkURUpiFvIZRgBmyArKCtzkjkZIvaBjMkXVbWGvbqzgexyALBsdjSGpngCwFkDifIBuufFMoWdiTskZoQJMqrTICTojIYxyeSxZyfroRODMbNDRZnPNRWCJPMHDtJmHAYORsUfUMApsVgzHblmYYtEjVgwfFbbGGcnqbaEREunUZjQXmZOtaRLUtmYgmSVYBADDvoxIfsfgPyCKmxIubeYTNDtjAyRRDedMiyLprucjiOgjhYeVwBTCMLfrDGXqwpzwVGqMZcLVCxaSJlDSYEofkkEYeqkKHqgBpnbPbgHMLUIDjUMmpBHCSjMJjxzuaiIsNBakqSwQpOQgNczgaczAInLqLIbAatLYHdaopovFOkqIexsFzXzrlcztxcdJJFuyZHRCovgpVvlGsXalGqARmneBZBFelhXkzzfNaVtAyyqWzKqQFbucqNJYWRncGKKLdTkNyoCSfkFohsVVxSAZWEXejhAquXdaaaZlRHoNXvpayoSsqcnCTuGZamCToZvPynaEphIdXaKUaqmBdtZtcOfFSPqKXSLEfZAPaJzldaUEdhITGHvBrQPqWARPXPtPVGNpdGERwVhGCMdfLitTqwLUecgOczXTbRMGxqPexOUAbUdQrIPjyQyQFStFubVVdHtAknjEQxCqkDIfTGXeJtuncbfqQUsXTOdPORvAUkAwww");

  tmp_s2 += std::string("TndUJHiQecbxzvqzlPWyqOsUbLnAIPRYxdkhCBcgRVdrSfOWLQrCNPANWvkkOdejVcugMVTnMUqBcVFohTtUTuWJRGiQjOTcVwHjKVtPoVVTYNXfTjPcDeMixVduzfhinMkGuotVuZJUuZkJwQsrzoVWaYedKZpEKrSOKAaKzKaWlfVcjYMcwVxDrnsfUrCFsVySzWtbpwvkbeRKvUdUyRSgDknQXeAJnkxLkNKWWUEqSfzvxvjTsnzIMKcSiStqDUZsoLEziJiWmAPKvATdjZyEvvmsIDSJeKovPdQSnyNSLTvmwZLbKtziYpTpzlFPhUhSWZdgpyWsRCkctWdcUosnpMVHZdiyeoUqzMJGHOxYTsbfqAqtXjkVItIBhgIvarcSbblmmHEfnwcNjQpkSlxkkUvYxTocDoTiUUJeVakGzAALtLhTUZHJOxKmRNwKDOUnXzmDikdJPukQLaxAHzFSnsZRqGNADbebYsIIwcKGFxUvaLskaPVQqUQnzXSfBigfpkmlDQoLjksSjFLLBLbuEhZFxQuvApLIPkPzXDPizdhpgJsjyBqaYFyqjyZdUShIIWpUYaqBhqHzoCIkjKQbHjIwqPZdSYRMmFeYRrkuEqCuPDMNeuiKTxRINKSzjAIvxBRDRLmciVEWlUwPHdPiHZuUiUFXrgXbHMbjeBvZwcJXyXLoetEAKetWXUDXJDUtrnryhmSlaOSSUVsZsmHKaNgeqzCZeWGRrDuCuWPtQAqMTyVfpvpkHdgWhxbhMSAcmaAvASRDtXBDGnMCDMOfzVWicMsnOkZlIUNSsCraKQBrSfkMAIStKMupnCMvADvezKymxCWrwlUxbdMAyhEcgCTauBGlunxyyOMAmrfxygzWgLcDnnMAWeJSYZBjGdAeQxmvLHqeeizRbUesPXmQumDoPOnnQVNsqKkejfUlxsWAwWjtpZBrWZKvdEANNUPkaNBORKcOQYawyaytedjZtrpZTljvjyNubgyU");

  tmp_s3 += std::string("rETWdtOoPuIaFONFwrxmPGDUaMiraBzMlulhxkKvSvnXEUmQuNfIqPjHTcdmcQRyRegnCPCePNnimQoCNrtHHZSHVyIifKqRwXVBMhXNhuqXwvAqhnQXRQSXFJLDKmsJvvoFlpVssIAYkbKrImXeoveCluuuNdNJGMnLVgpITBPZGJZlDkAcDeOtKifMofRksnOKExQBQzIWdJnunSelbYYOzQJYZuYsIIVQniApTAIwFjxvlksQbJajXgvWVWGCniuyxyhXphnvyQQHwnzDRITguovpoGAtItTvRaEDtMbmKEEksgqibDiuAMPEkWUVcnYIYLIFCmbXzmAumYLwoHOHbYhEMHUIaCnkiALLfQEADWUAUeUdAOQmFjxfAzsDjAsjwttyMUOopasuhTESsDemerTKSzUdvZOjUrNyHXgwcGZyYCDAokdrOQioJHWwqbeWPZBPYYlgTvJYNOnKOMIkroYgnQkALVSDTcUfAdtzlLwwZUBUQPrhVthbTtgjFepZBAdbGDyMThoDrRwkOKoYvEcEIkUZpqyyWpLpdgInRQXYUsryZTqryjyDaOtvUvVJhEZGuZITIVyonUOuTKhtdSGddzgtAHxOrdVEDlYNUzOTCyrcQnuMZnigcFJHUgNTnlLJkTgDCopMSpiJBFCPdPhsGJzkaoIJIxIUZrAIchbkWPjwjpZlWqgmJVyXXwFqkmXNMGpFlkYFlhYVpIyJaJBWtOfGtDtzZJfCTtmKiAJuMlZgAzFIbwZIDZXEiOGZCHZMIntqKThKtfIudpvNkMpptmrHQfeakFOeZddIQVFiRudbwUeMgoeHHrLiHQjgGbLjugFFMZMgFPNXVKpyMQyfrRaoxIsmSbkYdrnGTUJfiCkQZkLSuZBPWvvYnvTKcWRNaacuLGQcqNpPxvGOXZaTcxkUZRzJXLkfbeNGEgdnLLmXSYGluJVOpzrcMjBUWVaGbdRmifvVDtAgGbbfyLMreEphgESvQlJl");
  //gen_random_ref(1000, tmp_s1);
  //gen_random_ref(1000, tmp_s2);
  //gen_random_ref(1000, tmp_s3);

  tmp_s1 += std::string("plugin-100");
  tmp_s2 += std::string("plugin-101");
  tmp_s3 += std::string("plugin-102");
 
  headers.setCopy(LowerCaseString(std::string("user-plugin-agent-100")), tmp_s1);
  headers.setCopy(LowerCaseString(std::string("user-plugin-agent-101")), tmp_s2);
  headers.setCopy(LowerCaseString(std::string("user-plugin-agent-102")), tmp_s3);
  return FilterHeadersStatus::Continue;
}

FilterDataStatus TestRequestHeaderFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

void TestRequestHeaderFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  decoder_callbacks_ = &callbacks;
}

} // namespace Http
} // namespace Envoy
