#include "CLI/CLI.hpp"
#include <CLI/CLI.hpp>
#include <asset-packer.hpp>
#include <indicators/cursor_control.hpp>
#include <indicators/progress_bar.hpp>
#include <pack-reader.hpp>
#include <pack-writer.hpp>
#include <unistd.h>

void compressFiles (const std::string& path)
{
    AssetPacker::PackWriter writer ("./Assets.PAK");

    for (std::filesystem::recursive_directory_iterator i (path), end; i != end; ++i)
    {
        if (! is_directory (i->path()) && i->path().filename() != ".DS_Store")
        {
            writer.addFileToList (i->path());
        }
    }
    indicators::show_console_cursor (false);
    indicators::ProgressBar bar {
        indicators::option::BarWidth { 50 },
        indicators::option::Start { "[" },
        indicators::option::Fill { "#" },
        indicators::option::Lead { "$" },
        indicators::option::Remainder { "-" },
        indicators::option::End { " ]" },
        indicators::option::PostfixText {},
        indicators::option::ForegroundColor { indicators::Color::white },
        indicators::option::ShowPercentage { true },
        indicators::option::FontStyles { std::vector<indicators::FontStyle> { indicators::FontStyle::bold } }
    };

    int progressCounter = 0;
    writer.writePakFile ([&] (int numFiles, const std::string& fileName)
                         {
        int incAmount = 100 / numFiles;
        bar.set_option (indicators::option::PostfixText { "ProcessingFile: " + fileName });
        progressCounter += incAmount;
        bar.set_progress (progressCounter);
        std::this_thread::sleep_for (std::chrono::milliseconds (50));

        indicators::show_console_cursor (true); });
}

void uncompressFiles (const std::string& pakPath)
{
    AssetPacker::PackReader reader (pakPath);
    indicators::show_console_cursor (false);
    indicators::ProgressBar bar {
        indicators::option::BarWidth { 50 },
        indicators::option::Start { "[" },
        indicators::option::Fill { "#" },
        indicators::option::Lead { "$" },
        indicators::option::Remainder { "-" },
        indicators::option::End { " ]" },
        indicators::option::PostfixText {},
        indicators::option::ForegroundColor { indicators::Color::white },
        indicators::option::ShowPercentage { true },
        indicators::option::FontStyles { std::vector<indicators::FontStyle> { indicators::FontStyle::bold } }
    };

    int progressCounter = 0;
    reader.unpackAllFiles ("out/",
                           [&] (int numFiles, const std::string& filename)
                           {
                               int incAmount = 100 / numFiles;
                               bar.set_option (indicators::option::PostfixText { "ProcessingFile: " + filename });
                               progressCounter += incAmount;
                               bar.set_progress (progressCounter);
                               std::this_thread::sleep_for (std::chrono::milliseconds (50));
                           });

    indicators::show_console_cursor (true);
}

int main (int argc, char* argv[])
{
    CLI::App app { "Asset Packer \n A tool to build compressed pak files containing game assets" };
    argv = app.ensure_utf8 (argv);

    std::string folderPath;
    auto compress = app.add_subcommand ("compress", "Compress a given directory of files");
    compress->add_option ("folder", folderPath, "path to folder of assets");
    compress->callback ([&]()
                        { compressFiles (folderPath); });

    std::string pakFilePath;
    auto uncompress = app.add_subcommand ("uncompress", "uncompress a given PAK File.");
    uncompress->add_option ("pakfile", pakFilePath, "path to Pak File");
    uncompress->callback ([&]()
                          { uncompressFiles (pakFilePath); });
    CLI11_PARSE (app, argc, argv);
}
