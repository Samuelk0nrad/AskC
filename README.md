# AskC

Ask Command (*askc*) is a CLI program that lets you, as a beginner in the bash world, ask an AI how to perform various tasks. It helps to find the correct command for a specific task and explain how to use it, and how it works. *AskC* gives you practical examples and explains the most important parameters about the bash command you asked about.

## Usage

Just type `askc "The question you want to ask"` and then press enter, and that's it to ask a question in the terminal using *askc*!!!

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

