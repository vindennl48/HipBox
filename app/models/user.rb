class User < ApplicationRecord
  # Include default devise modules. Others available are:
  # :confirmable, :lockable, :timeoutable, :trackable and :omniauthable
  devise :database_authenticatable, :registerable,
         :recoverable, :rememberable, :validatable

  has_many     :channels
  has_many     :port_groups
  after_update :send_to_aem

  private

  def send_to_aem
    begin
      aem = { "mixer" => self }
      $OSCRUBY.send OSC::Message.new("/rails", aem.to_json)
    rescue
      puts "\n\nRAILS> Channel> OSC failed to send to AEM.. \n\n"
    end
  end
end
