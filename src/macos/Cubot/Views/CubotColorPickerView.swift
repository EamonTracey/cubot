import Cubot
import SwiftUI

struct CubotColorPickerView: View {
    @Binding var color: cubot.Color?

    var body: some View {
        HStack {
            Circle()
                .foregroundColor(.gray)
                .scaleEffect(self.color == nil ? 1.0 : 0.75)
                .onTapGesture { _ in self.color = nil }
            ForEach(cubot.Color.allCases, id: \.self) { color in
                Circle()
                    .foregroundColor(color.asSwiftUIColor)
                    .scaleEffect(self.color == color ? 1.0 : 0.75)
                    .onTapGesture { _ in self.color = color }
            }
        }
    }
}
