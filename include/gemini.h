#pragma once
#include <cpr/cpr.h>
#include <lib/json.hpp>
#include <request.h>
#include <string>

class Gemini : public Request {
public:
  Gemini(const std::string apiKey) { this->apikey = apiKey; };

  Response question(std::string &input) override {

    // clang-format off
    nlohmann::json bodyExample = {
        {"contents", {
            {
                {"parts", {
                    {{"text", input}}
                }}
            }
        }},
        {"generationConfig", {
            {"responseMimeType", "application/json"},
            {"responseSchema", {
                {"type", "OBJECT"},
                {"properties", {
                    {"generalExplanation", {{"type", "STRING"}}},
                    {"command", {{"type", "STRING"}}},
                    {"parameters", {
                        {"type", "ARRAY"},
                        {"items", {
                            {"type", "OBJECT"},
                            {"properties", {
                                {"parameter", {{"type", "STRING"}}},
                                {"description", {{"type", "STRING"}}}
                            }}
                        }}
                    }},
                    {"runnableCommand", {{"type","STRING"}}}
                }}
            }}
        }}
    };
    // clang-format on

    cpr::Response r =
        cpr::Post(cpr::Url{"https://generativelanguage.googleapis.com/v1beta/"
                           "models/gemini-2.5-flash:generateContent"},
                  cpr::Header{{"x-goog-api-key", this->apikey},
                              {"Content-Type", "application/json"}},
                  cpr::Body{bodyExample.dump()});

    try {
      nlohmann::json data = nlohmann::json::parse(r.text);
      std::string textOutput =
          data["candidates"][0]["content"]["parts"][0]["text"];

      nlohmann::json output = nlohmann::json::parse(textOutput);
      // std::cout << textOutput << std::endl;
      return Response(input, output);
    } catch (const nlohmann::json::parse_error &e) {
      return Response();
    }
    return Response();
  }
};
