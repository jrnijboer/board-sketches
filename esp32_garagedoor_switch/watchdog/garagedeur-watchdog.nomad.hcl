job "garagedeur-watchdog" {
  ui {
    description = "Garagedeur watchdog, monitors the mqtt topic for the garagedeur"
    link {
      label = "Sourcecode for this job"
      url   = "na"
    }
  }

  group "containers" {

    task "watchdog" {
      template {
        data          = <<EOF
{{ with nomadVar "nomad/jobs/garagedeur-watchdog" }}
MQTT_PASSWORD={{ .mqtt_password }}
HA_TOKEN={{ .ha_token }}
{{ end }}
EOF
        destination   = "local/env.vars"
        change_mode   = "restart"
        env           = true
      }

      driver = "docker"
      config {
        image = "jrnijboer/garagedeur:0.2"
      }

      env {
        TZ = "Europe/Amsterdam"
      }

      resources {
        memory = 50
      }
    }
  }
}
