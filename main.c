#include <wp/wp.h>
#include <gio/gio.h>
#include <pipewire/keys.h>
#include <pipewire/proxy.h>
#include <pipewire/device.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

void stop_all_players() {
        if (fork() == 0) {
                char *args[] = {"-a", "stop", "-s", NULL};
                fprintf(stderr, "Triggering...\n");
                execvp("playerctl", args);
        }
}

static void
handle_bluetooth_removed(WpObjectManager *om, gpointer obj, gpointer user_dat) {
        stop_all_players();
}

int
main() {
        wp_init (WP_INIT_ALL);

        GMainLoop *loop = g_main_loop_new (NULL, FALSE);
        WpCore *core = wp_core_new (NULL, wp_properties_new (
                                    PW_KEY_APP_NAME, "wpscript",
                                    NULL));

        WpObjectManager *bt_out_mgr = wp_object_manager_new();

        wp_object_manager_add_interest(bt_out_mgr, WP_TYPE_DEVICE,
                        WP_CONSTRAINT_TYPE_PW_GLOBAL_PROPERTY, "media.class", "=s", "Audio/Device",
                        WP_CONSTRAINT_TYPE_PW_PROPERTY, "device.api", "=s", "bluez5",
                        NULL);

        g_signal_connect(bt_out_mgr, "object-removed", G_CALLBACK(handle_bluetooth_removed), NULL);

        assert(wp_core_connect(core));

        wp_core_install_object_manager(core, bt_out_mgr);

        g_main_loop_run (loop);
        wp_core_disconnect (core);
}
