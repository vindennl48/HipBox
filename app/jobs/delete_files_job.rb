class DeleteFilesJob < ApplicationJob
  queue_as :default

  def perform(*args)
    puts "----> Deleting recorded files.. "

    folder = Time.now.strftime("%Y%m%d")
    files  = Dir["recorded/*"]

    if not files.empty?
      cmd = `mkdir -p recorded_bup/#{folder} && mv recorded/* recorded_bup/#{folder}/.`
    end

    puts "----> Deletion Complete!"
  end

end
