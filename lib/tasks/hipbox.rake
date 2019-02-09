namespace :hipbox do

  desc "Connect to LOF Wifi"
  task wifi: :environment do
    puts "----> Connecting to LOF Wifi.."
    cmd = `nmcli`

    is_successful = false

    if cmd[/LandOfFires/]
      puts "----> Already connected to LOF wifi!"
      is_successful = true
    else
      10.times do
        cmd = `nmcli device wifi list`
        if cmd[/LandOfFires/]
          cmd = `nmcli device wifi connect LandOfFires password p3trichor3`
          if cmd[/successfully activated/]
            puts "----> Connected successfully!"
            is_successful = true
          end
        else
          puts "  ##> Cant seem to find LOF wifi in range.."
        end

        if is_successful
          break
        else
          puts "  ##> Sleeping for 10 seconds ..."
          sleep(10)
        end
      end
    end

    if is_successful
      puts "    - Done!"
    else
      puts "----> Could not successfully connect to LOF wifi"
    end
  end

  desc "Upload Recorded Files"
  task upload: :environment do
    UploadRecordJob.perform_now
  end

end
