class Network < ApplicationRecord
  after_save :ensure_one_default

  private
    def ensure_one_default
      networks = Network.where.not(id: self.id)

      if self.default
        networks.each do |network|
          if network.default
            network.default = false
            network.save
          end
        end
      else
        if not networks.any? {|network| network.default}
          self.default = true
          self.save
        end
      end
    end
end
