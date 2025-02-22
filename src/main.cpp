#include <templatebot/templatebot.h>
#include <sstream>

/* When you invite the bot, be sure to invite it with the
 * scopes 'bot' and 'applications.commands', e.g.
 * https://discord.com/oauth2/authorize?client_id=940762342495518720&scope=bot+applications.commands&permissions=139586816064
 */

using json = nlohmann::json;

int main(int argc, char const *argv[])
{
    json configdocument;
    std::ifstream configfile("../config.json");
    configfile >> configdocument;

    /* Setup the bot */
    dpp::cluster bot(configdocument["token"].get<std::string>());

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());

    /* Handle slash command */
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
        else if (event.command.get_command_name() == "join") {
            dpp::guild* g = dpp::find_guild(event.command.guild_id);
	 
	        /* Attempt to connect to a voice channel, returns false if we fail to connect. */
	        if (!g->connect_member_voice(event.command.get_issuing_user().id)) {
	            event.reply("You don't seem to be in a voice channel!");
                return;
            }
            event.reply("Joined your channel!");
        }
    });

    /* Register slash command here in on_ready */
    bot.on_ready([&bot](const dpp::ready_t& event) {
        /* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
            bot.global_command_create(dpp::slashcommand("join", "Joins your voice channel.", bot.me.id));
        }
    });

    /* Start the bot */
    bot.start(dpp::st_wait);

    return 0;
}
