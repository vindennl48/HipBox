require 'google/apis/drive_v3'
require 'googleauth'
require 'googleauth/stores/file_token_store'
require 'fileutils'

class UploadRecordJob < ApplicationJob
  queue_as :default

  OOB_URI          = 'urn:ietf:wg:oauth:2.0:oob'.freeze
  APPLICATION_NAME = 'HipBox v6'.freeze
  CREDENTIALS_PATH = 'client_id.json'.freeze
  TOKEN_PATH       = 'token.yaml'.freeze
  SCOPE            = Google::Apis::DriveV3::AUTH_DRIVE
  DRIVE            = Google::Apis::DriveV3::DriveService.new

  FOLDER           = "application/vnd.google-apps.folder"
  IS_FOLDER        = "mimeType = '#{FOLDER}'"
  ROOT             = "'root' in parents"
  NOT_TRASH        = "trashed  = false"

  def perform(*args)

    def authorize
      client_id   = Google::Auth::ClientId.from_file(CREDENTIALS_PATH)
      token_store = Google::Auth::Stores::FileTokenStore.new(file: TOKEN_PATH)
      authorizer  = Google::Auth::UserAuthorizer.new(client_id, SCOPE, token_store)
      user_id     = 'default'
      credentials = authorizer.get_credentials(user_id)
      if credentials.nil?
        url = authorizer.get_authorization_url(base_url: OOB_URI)
        puts 'Open the following URL in the browser and enter the ' \
            "resulting code after authorization:\n" + url
        code = gets
        credentials = authorizer.get_and_store_credentials_from_code(
          user_id: user_id, code: code, base_url: OOB_URI
        )
      end
      credentials
    end

    # Initialize the API
    DRIVE.client_options.application_name = APPLICATION_NAME
    DRIVE.authorization                   = authorize

    # Variables for searching directories and performing queries
    def search_dir(dir)
      if dir.is_a? Hash
        result = "'#{dir[:id]}'"
      else
        result = "'#{dir}'"
      end
      return "#{result} in parents"
    end

    def list_dir(query)
      result = DRIVE.fetch_all(items: :files) do |page_token|
        DRIVE.list_files(
          q:          query,
          spaces:     'drive',
          fields:     'nextPageToken, files(id, name)',
          page_token: page_token
        )
      end
      return result
    end

    def make_query(options)
      result = ""
      options.each do |option|
        if result == ""
          result = option
        else
          result += " and #{option}"
        end
      end
      #puts "----> query result: #{result}"
      return result
    end

    # -- Uploading files --
    def create_hash(file)
      if file.is_a? Hash
        if not file.key? :id
          file[:id] = ""
        end
      else
        file = {name: file, id: ""}
      end
      return file
    end

    def create_dir(parent, child)
      parent = create_hash(parent)
      child  = create_hash(child)
      # -- Find existing folder or create new --
      files = list_dir(make_query([
        search_dir(parent),
        IS_FOLDER,
        NOT_TRASH,
      ]))

      for file in files
        if file.name == child[:name]
          child[:id] = file.id
        end
      end
      if child[:id] == ""
        folder_metadata = {
            name:      child[:name],
            mime_type: FOLDER,
            parents:   [parent[:id]],
        }
        child[:id] = DRIVE.create_file(folder_metadata, fields: 'id').id
      end
      return child
    end

    def create_file(parent, child)
      puts "----> Uploading '#{child[:name]}'..."

      parent = create_hash(parent)
      child  = create_hash(child)

      # -- Find existing folder or create new --
      files = list_dir(make_query([
        search_dir(parent),
        NOT_TRASH,
      ]))

      for file in files
        if file.name == child[:name]
          child[:id] = file.id
        end
      end

      if child[:id] == ""
        file_metadata = {
            name:      child[:name],
            parents:   [parent[:id]],
        }
        child[:id] = DRIVE.create_file(
          file_metadata,
          upload_source: "#{child[:source]}",
          content_type:  "audio/mpeg3",
          fields:        'id',
          options:       { retries: 10 },
        ).id
        puts "      Upload of '#{child[:name]}' complete!"
      else
        puts "  ##> File '#{child[:name]}' already exists!"
      end

      return child
    end

    def upload_files(file_list)
      folder_full_practice_recordings = {
        id:   "0ByRkDSWuaHQPeHBiTWJKdnc2Tjg",
        name: "Full Practice Recordings",
      }

      folder_today = create_dir(folder_full_practice_recordings, Time.now.strftime("%Y%m%d"))

      # -- Upload new files --
      file_list.each do |file_path|
        file      = file_path.split('/')[-1]
        numerator = file[/[0-9]{2}/]

        if file[/scratch/]
          upload_dir = folder_today
        else
          upload_dir = create_dir(folder_today, "scratch_#{numerator}")
        end

        create_file(upload_dir, {
          name:   file,
          source: file_path,
        })

      end

      puts "----> Finished uploading all files!"
    end

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

    def connect_and_upload(default, networks)
      if networks.empty?
        networks = [ default ]
      end

      puts "----> Connect and Upload in progress.."
      is_connected    = false
      connect_default = true

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
                is_connected    = true
                connect_default = false
              end

            end
          end
        end

        if is_connected
          break
        end
      end

      if is_connected
        puts "----> Starting upload"
        upload_all
        if connect_default
          puts "----> Re-connecting to network '#{default[:name]}'"
          connect(default[:name], default[:password])
        end
      else
        puts "  ##> Could not establish a solid connection to upload, please try again.."
      end

      puts "    - Done!"
    end

    def upload_all
      upload_files(Dir["recorded/*.mp3"])
    end


    ##################################################


    networks = Network.where.not(default: true)
    default  = Network.find_by(default: true)
    connect_and_upload(default, networks)

  end
end
