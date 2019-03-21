namespace :hipbox do

  desc "Connect to LOF Wifi"
  task wifi: :environment do

    def connect(wifi_name, wifi_passwd)
      10.times do
        cmd = `nmcli device wifi list`
        if cmd[/#{wifi_name}/]
          cmd = `nmcli device wifi connect #{wifi_name} password #{wifi_passwd}`
          if cmd[/successfully activated/]
            puts "----> Successfully connected to network '#{wifi_name}'!"
            sleep(10)
            return true
          else
            puts "  ##> Error when trying to connect to network '#{wifi_name}'"
          end
        else
          puts "  ##> Error when trying to find network '#{wifi_name}'"
        end
        puts "      Retrying in 10 seconds.."
        sleep(10)
      end
      return false
    end

    networks = Network.where.not(default: true)
    default  = Network.find_by(default: true)

    if networks.empty?
      networks = [ default ]
    end

    puts "----> Connect in progress.."
    is_connected = false

    10.times do
      cmd = `nmcli device wifi list`

      # if any of the listed networks match a found network
      if networks.any? {|network| cmd[/#{network[:name]}/]}
        networks.each do |network|
          if cmd[/#{network[:name]}/]

            cmd = `nmcli`

            # if not already connected to network
            if not cmd[/#{network[:name]}/]
              puts "----> Connecting to network '#{network[:name]}'"
              if connect(network[:name], network[:password])
                is_connected = true
              end

            # if already connected to network
            else
              puts "----> Already connected to network '#{network[:name]}'"
              is_connected = true
            end

          end
        end
      end

      if is_connected
        break
      end
    end

    puts "    - Done!"
  end

  desc "Upload Recorded Files"
  task upload: :environment do
    UploadRecordJob.perform_now
  end

end
