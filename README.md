# AskC

Ask Command (*askc*) is a CLI program that lets you, as a beginner in the bash world, ask an AI how to perform various tasks. It helps to find the correct command for a specific task and explain how to use it, and how it works. *AskC* gives you practical examples and explains the most important parameters about the bash command you asked about.

## Usage

Just type `askc "The question you want to ask"` and then press enter, and that's it to ask a question in the terminal using *askc*!!!

You will get a small explanation about the command. A general command with placeholders, parameters, and flags. And at the end, multiple ready-to-use example commands you can directly try and test. Using the arrow keys, you can move between the general and ready-to-use commands by pressing `enter`, the command will get saved into your clipboard, and you can use it directly.

### Settings

To configure *askc*, just use that keyword config like this: `askc config`. This will get you to a menu where you can configure *askc*. You can change the provider settings, like the name of the provider and the api key. And of course, you can set the provider that should respond if you ask a question. (Please note that currently only Gemini is integrated)

## Set up

### Installation

Just go to the latest release on GitHub and download the executable for your machine, and run it!!!
To make it easier for you, you can add the executable to your environment variables, so you just have to run `askc` and not `./askc`

### Configuration

Before you run the program for the first time, you may add your api key for your preferred provider and set it as the default (currently only Gemini is supported)

1. Run `askc config` to enter the configuration menu
2. Select `config provider` and add your api key and the name of your provider
3. Then you have to set the provider as the default

## Program documentation

The entry point into the application is the main function in the [askc.cpp](src/askc.cpp) file. If the user used the 'config' keyword, the program will enter the settings menu in (settings.cpp)[src/settings/settings.cpp].

The project is structured so that all the settings-related things are in the src/settings folder, and the header files are in the include folder.

file structure:

This file outlines the most important files and folders in the AskC project, focusing on the entry point, settings management, and the include directory for core headers.
``` (ai generated)
AskC/
├── src/
│   ├── askc.cpp                 # Main entry point of the application
│   └── settings/
│       ├── settings.cpp         # Main settings logic (menu, provider, API key)
│       ├── ...                  # (other settings-related source files)
├── include/
│   ├── gemini.h                 # Gemini API integration and request logic
│   ├── request.h                # Terminal interaction, response handling, UI helpers
│   └── ...                      # (other public API headers)
├── README.md                    # Project overview and instructions
├── test.cpp                     # Test and demo code for terminal UI and features
└── ...                          # (other files and folders)
```
Additional files and folders exist, but these are the core for understanding and modifying AskC's main logic.

### Gemini integration

model: gemini-2.5-flash

I use the [nlohmann JSON library](https://github.com/nlohmann/json) for the JSON in the request and response of the gemini api.

This is the expected JSON response that Gemini should return:

``` JSON
{
  "type": "OBJECT",
  "properties": {
    "generalExplanation": {
      "type": "STRING",
      "description": "A general explanation of the command or concept."
    },
    "command": {
      "type": "STRING",
      "description": "The main shell command as text."
    },
    "parameters": {
      "type": "ARRAY",
      "description": "List of parameters for the command and their descriptions.",
      "items": {
        "type": "OBJECT",
        "properties": {
          "parameter": { "type": "STRING", "description": "Parameter name." },
          "description": { "type": "STRING", "description": "Parameter description." }
        }
      }
    },
    "runnableCommand": {
      "type": "STRING",
      "description": "A runnable, copy-paste-ready command."
    }
  }
}
```

The request gets sent to the gemini api to `https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash:generateContent`.

The response gets parsed into usable data and then displayed using the [request.h](include/request.h)

### UI  <-- AI helped a lot with creating the CLI's UI and user experience, like the navigation using the arrow keys

#### [request.h](include/request.h)

The purpose of this file is to define the classes and the functions for the Terminal manipulation, like color and text, for an interactive interface.
`ANCI` codes are defined to style and format the text of text in the interface.

